import os
import sys

import pyqtgraph as pg
import matplotlib.pyplot as plt
import numpy as np

import copy_dataset

from scipy.fft import fft, ifft
from collections import Counter
from tensorflow.keras.models import load_model
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import (QWidget, QPushButton,
                             QLabel, QVBoxLayout,
                             QHBoxLayout, QLineEdit,
                             QMainWindow, QFileDialog,
                             QListWidget, QMessageBox)

from Fourier import Fourier
from work_with_osc import DataOsc
from work_with_csv import my_csv
from test_model import TestModel

# Теперь импортируем модуль для работы с osc
module_path = os.path.abspath("cpp/build/Debug")
sys.path.append(module_path)
try:
    import Aegis_osc
except ImportError as e:
    raise "Не удалось импортировать модуль для работы с осциллограммами!"


class OscCateg(QMainWindow):
    def __init__(self):
        super().__init__()
        self.main_widjet = QWidget()
        self.main_layout_v = QVBoxLayout()
        self.main_widjet.setLayout(self.main_layout_v)

        self.bttn_open_osc = QPushButton("Открыть осциллограмму")
        self.bttn_open_osc.clicked.connect(self.__on_open_osc)

        self.main_layout_v.addWidget(self.bttn_open_osc)
        self.setCentralWidget(self.main_widjet)
        self.show()

    def __on_open_osc(self) -> None:
        """Обработчик события клика для кнопки создания нового датасета"""
        self.path_file, _ = QFileDialog.getOpenFileName(self, "Выберите файл", "", "OSC Files (*.osc)")
        try:
            if hasattr(self, "bttn_best_osc"):
                self.bttn_best_osc.deleteLater()
                self.bttn_good_osc.deleteLater()
                self.bttn_check_osc.deleteLater()
                self.bttn_noise_osc.deleteLater()
                self.bttn_vibration_osc.deleteLater()
                self.categ_layout_h.deleteLater()
                del self.bttn_best_osc
                del self.bttn_good_osc
                del self.bttn_check_osc
                del self.bttn_noise_osc
                del self.bttn_vibration_osc
                del self.categ_layout_h

            self.osc_file = Aegis_osc.File_osc(self.path_file)
            self.num_osc = self.osc_file.m_sdoHdr.NumOSC
            self.osc_datas = []
            self.start_data_osc = 0
            self.end_data_osc = 500 if self.num_osc > 500 else self.num_osc
            self.osc_datas.extend(self.osc_file.getDotsOSC(0, self.end_data_osc))

            self.plot_layout_h = QHBoxLayout(self)
            self.main_layout_v.addLayout(self.plot_layout_h)
            self.osc_now = 0
            if not hasattr(self, "now_plot"):
                self.now_plot = pg.PlotWidget(self)
                self.plot_layout_h.addWidget(self.now_plot)
                self.now_plot.setStyleSheet("min-height: 250px; max-height: 400px; min-width: 600px")
            self.now_plot.clear()
            self.now_plot.plot(self.osc_datas[self.osc_now])

            # self.plot_layout_h.addWidget(pg.plot(self.osc_datas[self.osc_now]))
            if (not hasattr(self, "bttn_next_osc") and
                    not hasattr(self, "bttn_prev_osc")):
                self.bttn_next_osc = QPushButton("Следующая осциллограмма")
                self.bttn_prev_osc = QPushButton("Предыдущая осциллограмма")
                self.bttn_next_osc.clicked.connect(self.__open_next_osc)
                self.bttn_prev_osc.clicked.connect(self.__open_prev_osc)
                self.main_layout_v.addWidget(self.bttn_next_osc)
                self.main_layout_v.addWidget(self.bttn_prev_osc)
            self.__check_next_prev_osc()

            if not hasattr(self, "bttn_predict"):
                self.bttn_predict = QPushButton("Разбить по категориям")
                self.bttn_predict.clicked.connect(self.__on_bttn_predict)
                self.main_layout_v.addWidget(self.bttn_predict)

        except Exception as e:
            print(f"Ошибка при открытии файла OSC: {e}")

    def __load_next_osc(self):
        if self.osc_now >= self.num_osc - 1:
            return
        # self.osc_datas = []
        # self.start_data_osc = 0
        self.end_data_osc = self.end_data_osc + 500 if (self.num_osc - self.osc_now) > 500 else self.num_osc - 1
        self.osc_datas.extend(self.osc_file.getDotsOSC(0, self.end_data_osc))

    def __check_next_prev_osc(self) -> None:
        if self.osc_now + 1 >= self.num_osc:
            self.bttn_next_osc.setEnabled(False)
        elif not self.bttn_next_osc.isEnabled():
            self.bttn_next_osc.setEnabled(True)
        if self.osc_now <= self.start_data_osc:
            self.bttn_prev_osc.setEnabled(False)
        elif not self.bttn_prev_osc.isEnabled():
            self.bttn_prev_osc.setEnabled(True)

    def __open_next_osc(self) -> None:
        if self.osc_now >= self.end_data_osc - 1:
            self.__load_next_osc()
            self.__check_next_prev_osc()
            if not self.bttn_next_osc.isEnabled():
                return

        self.osc_now += 1
        self.now_plot.clear()
        self.now_plot.plot(self.osc_datas[self.osc_now])
        self.__check_next_prev_osc()

    def __open_prev_osc(self) -> None:
        if self.osc_now <= self.start_data_osc:
            self.__check_next_prev_osc()
            return
        self.osc_now -= 1
        self.now_plot.clear()
        self.now_plot.plot(self.osc_datas[self.osc_now])
        self.__check_next_prev_osc()

    def __on_bttn_predict(self):
        model = TestModel()
        self.pedict_res = model.predict(self.path_file)

        if not hasattr(self, "bttn_best_osc"):
            self.categ_layout_h = QHBoxLayout()

            self.bttn_best_osc = QPushButton(f"Лучшие {len(self.pedict_res["Best"])}шт.")
            self.bttn_good_osc = QPushButton(f"Хорошие {len(self.pedict_res["Good"])}шт.")
            self.bttn_check_osc = QPushButton(f"Перепроверка {len(self.pedict_res["Check"])}шт.")
            self.bttn_noise_osc = QPushButton(f"Шум {len(self.pedict_res["Noise"])}шт.")
            self.bttn_vibration_osc = QPushButton(f"Вибрация {len(self.pedict_res["Vibration"])}шт.")

            self.bttn_best_osc.clicked.connect(self.__on_load_categ_osc)
            self.bttn_good_osc.clicked.connect(self.__on_load_categ_osc)
            self.bttn_check_osc.clicked.connect(self.__on_load_categ_osc)
            self.bttn_noise_osc.clicked.connect(self.__on_load_categ_osc)
            self.bttn_vibration_osc.clicked.connect(self.__on_load_categ_osc)

            self.categ_layout_h.addWidget(self.bttn_best_osc)
            self.categ_layout_h.addWidget(self.bttn_good_osc)
            self.categ_layout_h.addWidget(self.bttn_check_osc)
            self.categ_layout_h.addWidget(self.bttn_noise_osc)
            self.categ_layout_h.addWidget(self.bttn_vibration_osc)

            self.main_layout_v.addLayout(self.categ_layout_h)

    def __on_load_categ_osc(self):
        bttn = self.sender()

        if bttn == self.bttn_best_osc:
            self.load_osc("Best")
        elif bttn == self.bttn_good_osc:
            self.load_osc("Good")
        elif bttn == self.bttn_check_osc:
            self.load_osc("Check")
        elif bttn == self.bttn_noise_osc:
            self.load_osc("Noise")
        elif bttn == self.bttn_vibration_osc:
            self.load_osc("Vibration")

    def load_osc(self, category: str) -> None:
        if not hasattr(self, "path_file"):
            QMessageBox.critical(self, "не открыт файл", "Вы не открыли никакого файла .osc")
        if len(self.pedict_res[category]) == 0:
            QMessageBox.information(self, "Категория пуста", "Нет элементов данной категории")
            return
        file_name = (f"{self.path_file[:self.path_file.rfind(".")]}"
                     + f"_{category}"
                     + f"{self.path_file[self.path_file.rfind("."):]}")
        self.osc_file.openReadFile()
        list_osc = [self.osc_file.getDotOSC_NOC(el) for el in self.pedict_res[category]]
        list_indexes = [el for el in self.pedict_res[category]]
        self.osc_file.cls()
        self.osc_file.saveOSC(file_name, list_osc, list_indexes)


if __name__ == "__main__":
    try:
        app = QtWidgets.QApplication([])
        ex = OscCateg()
        sys.exit(app.exec())
    except Exception as e:
        print(e)

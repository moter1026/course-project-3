import inspect
import multiprocessing
import os
import sys
import logging
import pyqtgraph as pg

from PyQt5 import QtWidgets
from PyQt5.QtWidgets import (QWidget, QPushButton,
                             QVBoxLayout, QHBoxLayout,
                             QMainWindow, QFileDialog,
                             QMessageBox)

import Aegis_osc

from seeOSC import SeeOSC
from test_model import TestModel

# Настройка логирования
# logging.basicConfig(
#     level=logging.DEBUG,
#     format='%(asctime)s - %(levelname)s - %(message)s',
#     handlers=[
#         logging.FileHandler("application.log", encoding='utf-8'),
#         logging.StreamHandler(sys.stdout)
#     ]
# )
LOG_LEVEL = Aegis_osc.LogLevel

class OscCateg(QMainWindow):
    def __init__(self):
        super().__init__()
        self.main_widjet = QWidget()
        self.main_layout_v = QVBoxLayout()
        self.main_widjet.setLayout(self.main_layout_v)

        self.seeOSC = SeeOSC()
        self.logger = self.seeOSC.menu.logger
        # Подключаем сигнал от графического виджета
        self.seeOSC.menu.graphs_shown.connect(self.__on_graphs_shown)

        self.main_layout_v.addWidget(self.seeOSC)
        self.setCentralWidget(self.main_widjet)
        self.show()
        self.logger.logg(LOG_LEVEL._INFO_, "Приложение OscCateg запущено.",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
        # logging.info("Приложение OscCateg запущено.")

    def __on_graphs_shown(self) -> None:
        self.path_file = self.seeOSC.menu.name_osc
        if not hasattr(self, "bttn_predict"):
            self.bttn_predict = QPushButton("Разбить по категориям")
            self.bttn_predict.clicked.connect(self.__on_bttn_predict)
            self.main_layout_v.addWidget(self.bttn_predict)

        if hasattr(self, "bttn_best_osc"):
            self.__remove_old_widgets()

    def __load_next_osc(self):
        if self.osc_now >= self.num_osc - 1:
            return
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

    def __remove_old_widgets(self):
        """Удаление старых виджетов категорий при открытии нового файла."""
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
        self.logger.logg(LOG_LEVEL._INFO_, "Старые виджеты категорий удалены.",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)

    def __on_bttn_predict(self):
        try:
            model = TestModel()
            self.pedict_res = model.predict(self.path_file, self.logger)
            self.logger.logg(LOG_LEVEL._INFO_, f"Результаты предсказания получены: {self.pedict_res.keys()}",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)

            if not hasattr(self, "bttn_best_osc"):
                self.__create_category_buttons()
        except Exception as e:
            self.logger.logg(LOG_LEVEL._ERROR_, f"Ошибка при предсказании категорий: {e}",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            QMessageBox.critical(self, "Ошибка", f"Ошибка при предсказании: {e}")

    def __create_category_buttons(self):
        """Создание кнопок для категорий осциллограмм."""
        self.categ_layout_h = QHBoxLayout()

        self.bttn_best_osc = QPushButton(f"Лучшие {len(self.pedict_res['Best'])}шт.")
        self.bttn_good_osc = QPushButton(f"Хорошие {len(self.pedict_res['Good'])}шт.")
        self.bttn_check_osc = QPushButton(f"Перепроверка {len(self.pedict_res['Check'])}шт.")
        self.bttn_noise_osc = QPushButton(f"Шум {len(self.pedict_res['Noise'])}шт.")
        self.bttn_vibration_osc = QPushButton(f"Вибрация {len(self.pedict_res['Vibration'])}шт.")

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
        self.logger.logg(LOG_LEVEL._INFO_, "Кнопки категорий созданы.",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)

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
        self.seeOSC.menu.osc_file.openReadFile()
        list_osc = [self.seeOSC.menu.osc_file.getDotOSC_NOC(el) for el in self.pedict_res[category]]
        list_indexes = [el for el in self.pedict_res[category]]
        self.seeOSC.menu.osc_file.cls()
        # Переделать метод saveOSC.Передавать только list_indexes
        print(f"before save file")
        self.seeOSC.menu.osc_file.saveOSC(file_name, list_osc, list_indexes)
        self.seeOSC.menu.ald_file.saveNewAld(file_name)
        print(f"after save file")


if __name__ == "__main__":
    from multiprocessing import freeze_support
    freeze_support()  # Для совместимости с PyInstaller на Windows
    try:
        app = QtWidgets.QApplication([])
        ex = OscCateg()
        sys.exit(app.exec())

    except Exception as e:
        QMessageBox.critical(None, "Критическая ошибка", f"Критическая ошибка: {e}")
        sys.exit(1)
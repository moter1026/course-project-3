import inspect
import os
import sys

import numpy as np
import pyqtgraph as pg

from numba import njit
from PyQt5 import QtWidgets
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtWidgets import (QWidget, QPushButton,
                             QLabel, QVBoxLayout,
                             QHBoxLayout, QMessageBox,
                             QMainWindow, QFileDialog)

import Aegis_osc

from Fourier import Fourier, four2
from work_with_osc import DataOsc, get_dB_osc


LOG_LEVEL = Aegis_osc.LogLevel

@njit
def set_K_mkV_and_dB(end_data_osc: int, K_mkV: np.ndarray, osc_datas: np.ndarray) -> np.ndarray:
    """Метод рассчитывает Децибелы для осциллограммы"""
    dB_data = np.zeros(end_data_osc, dtype=np.int32)  # Массив с нужной длиной
    for i in range(end_data_osc):
        dB_data[i] = np.int32(get_dB_osc(osc_datas[i], K_mkV[i]))
    return dB_data

class MainMenu(QWidget):
    # Создаем сигнал, который уведомит об отображении графиков
    graphs_shown = pyqtSignal()

    def __init__(self):
        super().__init__()  # Вызываю конструктор родительского класса QWidget
        self.logger = Aegis_osc.Logger("log_seeOSC.txt")

        self.dB_text = None
        self.text = None
        self.now_plot_spectr_item = None
        self.now_plot_osc_item = None
        self.main_layout_v = QVBoxLayout(self)

        # Кнопка для получения открытия отдельной осциллограммы
        self.bttn_open_alone_osc = QPushButton()
        self.bttn_open_alone_osc.setText("открыть ald и osc")
        self.bttn_open_alone_osc.clicked.connect(self._on_clicked_bttn_open_ald_and_osc)

        self.file_open = QLabel(self)
        self.info_now_num_osc = QLabel(self)

        # Блок добавления элементов в главное меню
        self.main_layout_v.addWidget(self.bttn_open_alone_osc)

    def _on_clicked_bttn_open_ald_and_osc(self) -> None:
        """Обработчик события клика для кнопки создания нового датасета"""
        path, _ = QFileDialog.getOpenFileName(self, "Выберите файл", "", "ald files (*.ald)")

        self.__open_osc(path)

    def __open_osc(self, name_ald: str):
        try:
            self.logger.logg(LOG_LEVEL._INFO_, f"Открытие файла .ald: {name_ald}",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            # Инициализация переменных
            self.name_ald = name_ald
            # self.file_open.setText(f"Открыт: {self.name_ald}")
            self.ald_file = Aegis_osc.File_ald(name_ald, self.logger)
            self.name_osc: str = self.ald_file.measData.get_sdo_name()
            if self.name_osc == "":
                raise "Нет привязанного файла осциллограмм к файлу ald."
            elif self.name_osc.find("/") != -1:
                self.name_osc = self.name_osc[self.name_osc.rfind("/") + 1:]
            elif  self.name_osc.find("\\") != -1:
                self.name_osc = self.name_osc[self.name_osc.rfind("\\") + 1:]
            
            self.name_osc = self.name_ald[:self.name_ald.rfind("/") + 1] + self.name_osc
            self.logger.logg(LOG_LEVEL._INFO_, f"Открытие файла .osc: {self.name_osc}",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            self.osc_file = Aegis_osc.File_osc(self.name_osc, self.logger)

            self.file_open.setText(f"Открыт файл .ald: {self.name_ald}\nОткрыт файл .osc: {self.name_osc}")
            self.num_osc = self.osc_file.sdoHdr.NumOSC

            # Предварительная инициализация массивов
            self.start_data_osc = 0
            self.end_data_osc = min(self.num_osc, 500)

            # Получаем данные и инициализируем массивы заранее
            self.logger.logg(LOG_LEVEL._INFO_, f"Получение данных из файла .osc",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            self.osc_datas = np.array(self.osc_file.getDotsOSC(0, self.end_data_osc))  
                      
            # Заполняем массивы за один проход, без использования np.append
            self.logger.logg(LOG_LEVEL._INFO_, f"Старт получения K_mkV",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            self.K_mkV = np.array(self.osc_file.get_K_mkV(0, self.end_data_osc))
            self.logger.logg(LOG_LEVEL._INFO_, f"Конец получения K_mkV",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            self.logger.logg(LOG_LEVEL._INFO_, f"Старт получения децибел",
                    os.path.basename(__file__),
                    inspect.currentframe().f_lineno, 
                    inspect.currentframe().f_code.co_name)
            self.dB_data = set_K_mkV_and_dB(self.end_data_osc, self.K_mkV, self.osc_datas)
            self.logger.logg(LOG_LEVEL._INFO_, f"Конец получения децибел",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            self.plot_layout_h = QHBoxLayout(self)
            self.osc_now = 0
            if not hasattr(self, "now_plot_osc"):
                self.logger.logg(LOG_LEVEL._INFO_, f"Создание графиков и их отображение",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
                self.now_plot_osc = pg.PlotWidget(self)
                self.now_plot_spectr = pg.PlotWidget(self)

                self.now_plot_osc.setTitle("Осциллограмма")
                self.now_plot_spectr.setTitle("Спектр осциллограммы")
                self.now_plot_osc.showGrid(x=True, y=True)
                self.now_plot_spectr.showGrid(x=True, y=True)

                self.plot_layout_h.addWidget(self.now_plot_osc)
                self.plot_layout_h.addWidget(self.now_plot_spectr)
                self.now_plot_osc.setStyleSheet("min-height: 250px; max-height: 400px; min-width: 600px")
                self.now_plot_spectr.setStyleSheet("min-height: 250px; max-height: 400px; min-width: 600px")

                self.main_layout_v.addWidget(self.file_open)
                self.main_layout_v.addWidget(self.info_now_num_osc)

                self.main_layout_v.addLayout(self.plot_layout_h)

            self.now_plot_osc.clear()
            self.now_plot_spectr.clear()

            self.info_now_num_osc.setText(f"Номер кадра: {self.osc_now + 1} из {self.num_osc}")
            self.now_plot_osc_item = self.now_plot_osc.plot(self.osc_datas[self.osc_now])

            self.logger.logg(LOG_LEVEL._INFO_, f"Построение спектра осциллограммы",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            spectr = four2(np.array(self.osc_datas[self.osc_now], dtype=np.float64))
            x = np.array([i / (len(spectr) / 2 / 500) for i in range(round(len(spectr) / 2))])
            self.now_plot_spectr_item = self.now_plot_spectr.plot(x, spectr[:round(len(spectr) / 2)])
            # Добавляем информацию о том, какая частота более выражена в сигнале
            index = np.argmax(spectr[:round(len(spectr) / 2)])
            self.text = pg.TextItem(f"{x[index]} кГц", anchor=(0, 1), color="r")
            self.now_plot_spectr.addItem(self.text)

            # Связываем изменение диапазона графика с обновлением позиции текста
            self.now_plot_spectr.sigRangeChanged.connect(self.__update_info_position)

            # Добавляем информацию о децибелах
            self.dB_text = pg.TextItem(f"{self.dB_data[self.osc_now]} Дб", anchor=(0, 1), color="r")
            self.now_plot_osc.addItem(self.dB_text)

            self.__update_info_position()

            # Связываем изменение диапазона графика с обновлением позиции текста
            self.now_plot_osc.sigRangeChanged.connect(self.__update_info_position)

            # self.plot_layout_h.addWidget(pg.plot(self.osc_datas[self.osc_now]))
            if (not hasattr(self, "bttn_next_osc") and
                    not hasattr(self, "bttn_prev_osc")):
                self.logger.logg(LOG_LEVEL._INFO_, f"Создание кнопок для переключения между осциллограммами",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
                self.bttn_next_osc = QPushButton("Следующая осциллограмма")
                self.bttn_prev_osc = QPushButton("Предыдущая осциллограмма")
                self.bttn_next_osc.clicked.connect(self.open_next_osc)
                self.bttn_prev_osc.clicked.connect(self.open_prev_osc)
                self.main_layout_v.addWidget(self.bttn_next_osc)
                self.main_layout_v.addWidget(self.bttn_prev_osc)
            self.check_next_prev_osc()
            # Отправляем сигнал о том, что графики были отображены
            self.graphs_shown.emit()
        except Exception as e:
            self.logger.logg(LOG_LEVEL._INFO_, f"Ошибка при открытии файла OSC: {e}",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)

    def __get_max_boundaries_in_plot(self, graph) -> (float, float):
        view_range = self.now_plot_osc.viewRange()  # Получаем границы видимой области
        return view_range[0][1], view_range[1][1]  # Максимальные значения по оси X и Y

    def __get_min_boundaries_in_plot(self, graph) -> (float, float):
        view_range = self.now_plot_osc.viewRange()  # Получаем границы видимой области
        return view_range[0][0], view_range[1][0]  # Минимальные значения по оси X и Y

    def __update_info_position(self) -> None:
        """Обновляет позицию текстового элемента в правом верхнем углу графика."""
        graph = self.sender()

        if graph == self.now_plot_osc:
            y_min, y_max = self.now_plot_osc_item.dataBounds(1)
            x_min, x_max = self.now_plot_osc_item.dataBounds(0)
            x_width = x_max - x_min
            y_height = y_max - y_min
            delta_x = x_width * 0.85
            delta_y = y_height * 0.9
            # Устанавливаем текст в правый верхний угол с небольшим отступом
            self.dB_text.setPos(x_min + delta_x, y_min + delta_y)
            return
        elif graph == self.now_plot_spectr:
            y_min, y_max = self.now_plot_spectr_item.dataBounds(1)
            x_min, x_max = self.now_plot_spectr_item.dataBounds(0)
            x_width = x_max - x_min
            y_height = y_max - y_min
            delta_x = x_width * 0.85
            delta_y = y_height * 0.9
            # Устанавливаем текст в правый верхний угол с небольшим отступом
            self.text.setPos(x_min + delta_x, y_min + delta_y)
            return
        else:
            # Если метод вызван напрямую, то устанавливаем инфо-текст в верхний правый угол у обоих графиков
            # для self.now_plot_osc
            y_min, y_max = self.now_plot_osc_item.dataBounds(1)
            x_min, x_max = self.now_plot_osc_item.dataBounds(0)
            x_width = x_max - x_min
            y_height = y_max - y_min
            delta_x = x_width * 0.85
            delta_y = y_height * 0.9
            # Устанавливаем текст в правый верхний угол с небольшим отступом
            self.dB_text.setPos(x_min + delta_x, y_min + delta_y)

            # для self.now_plot_spectr
            y_min, y_max = self.now_plot_spectr_item.dataBounds(1)
            x_min, x_max = self.now_plot_spectr_item.dataBounds(0)
            x_width = x_max - x_min
            y_height = y_max - y_min
            delta_x = x_width * 0.85
            delta_y = y_height * 0.9
            # Устанавливаем текст в правый верхний угол с небольшим отступом
            self.text.setPos(x_min + delta_x, y_min + delta_y)
        return

    def __load_prev_osc(self):
        if self.osc_now <= 0:
            return
        self.end_data_osc = self.start_data_osc
        self.start_data_osc = self.start_data_osc - 500 if (self.start_data_osc - 500) > 0 else 0
        self.osc_datas = np.array(self.osc_file.getDotsOSC(self.start_data_osc, self.end_data_osc))

        self.K_mkV = np.array(self.osc_file.get_K_mkV(self.start_data_osc, self.end_data_osc))
        self.dB_data = np.array(([get_dB_osc(self.osc_datas[i], self.K_mkV[i])
                                  for i in range(0, self.end_data_osc - self.start_data_osc)]))

    def __load_next_osc(self):
        if self.osc_now >= self.num_osc - 1:
            return
        self.start_data_osc = self.end_data_osc
        self.end_data_osc = self.end_data_osc + 500 if (self.num_osc - self.osc_now) > 500 else self.num_osc
        self.osc_datas = np.array(self.osc_file.getDotsOSC(self.start_data_osc, self.end_data_osc))

        self.K_mkV = np.array(self.osc_file.get_K_mkV(self.start_data_osc, self.end_data_osc))
        self.dB_data = np.array(([get_dB_osc(self.osc_datas[i], self.K_mkV[i])
                                  for i in range(0, self.end_data_osc - self.start_data_osc)]))

    def keyPressEvent(self, event):
        """
        Переопределение метода обработки событий нажатия клавиш
        """
        if event.key() == Qt.Key_Left:
            self.open_prev_osc()
        elif event.key() == Qt.Key_Right:
            self.open_next_osc()

    def check_next_prev_osc(self) -> None:
        if self.osc_now + 1 >= self.num_osc:
            self.bttn_next_osc.setEnabled(False)
        elif not self.bttn_next_osc.isEnabled():
            self.bttn_next_osc.setEnabled(True)
        if self.osc_now <= self.start_data_osc:
            self.bttn_prev_osc.setEnabled(False)
        elif not self.bttn_prev_osc.isEnabled():
            self.bttn_prev_osc.setEnabled(True)

    def open_next_osc(self) -> None:
        if self.osc_now >= self.end_data_osc - 1:
            self.__load_next_osc()
            self.check_next_prev_osc()
            if not self.bttn_next_osc.isEnabled():
                return

        self.osc_now += 1
        self.now_plot_osc.clear()
        self.now_plot_spectr.clear()

        self.info_now_num_osc.setText(f"Номер кадра: {self.osc_now + 1} из {self.num_osc}")

        now_ind = self.osc_now - self.start_data_osc
        self.now_plot_osc_item = self.now_plot_osc.plot(self.osc_datas[now_ind])
        spectr = four2(np.array(self.osc_datas[now_ind], dtype=np.float64))
        x = [i / (len(spectr) / 2 / 500) for i in range(round(len(spectr) / 2))]
        self.now_plot_spectr_item = self.now_plot_spectr.plot(x, spectr[:round(len(spectr) / 2)])

        # Добавляем информацию о том, какя частота более выражена в сигнале
        index = np.argmax(spectr[:round(len(spectr) / 2)])
        self.text = pg.TextItem(f"{x[index]} кГц", anchor=(0, 1), color="r")
        self.now_plot_spectr.addItem(self.text)

        # Добавляем информацию децибелах
        self.dB_text = pg.TextItem(f"{self.dB_data[now_ind]} Дб", anchor=(0, 1), color="r")
        self.now_plot_osc.addItem(self.dB_text)

        self.__update_info_position()
        self.check_next_prev_osc()

    def open_prev_osc(self) -> None:
        if self.osc_now <= self.start_data_osc - 1:
            self.__load_prev_osc()
            self.check_next_prev_osc()
            if not self.bttn_prev_osc.isEnabled():
                return
        self.osc_now -= 1
        self.now_plot_osc.clear()
        self.now_plot_spectr.clear()

        self.info_now_num_osc.setText(f"Номер кадра: {self.osc_now + 1} из {self.num_osc}")

        now_ind = self.osc_now - self.start_data_osc
        self.now_plot_osc_item = self.now_plot_osc.plot(self.osc_datas[now_ind])
        spectr = four2(np.array(self.osc_datas[now_ind], dtype=np.float64))
        x = [i / (len(spectr) / 2 / 500) for i in range(round(len(spectr) / 2))]
        self.now_plot_spectr_item = self.now_plot_spectr.plot(x, spectr[:round(len(spectr) / 2)])

        # Добавляем текст (аннотацию)
        index = np.argmax(spectr[:round(len(spectr) / 2)])
        self.text = pg.TextItem(f"{x[index]} кГц", anchor=(0, 1), color="r")
        self.now_plot_spectr.addItem(self.text)

        # Добавляем информацию децибелах
        self.dB_text = pg.TextItem(f"{self.dB_data[now_ind]} Дб", anchor=(0, 1), color="r")
        self.now_plot_osc.addItem(self.dB_text)

        self.__update_info_position()
        self.check_next_prev_osc()


class SeeOSC(QMainWindow):
    def __init__(self):
        super().__init__()
        self.menu = None
        self.create_menu()

    def create_menu(self):
        self.menu = MainMenu()

        self.setCentralWidget(self.menu)
        self.show()


if __name__ == "__main__":
    try:
        app = QtWidgets.QApplication([])
        ex = SeeOSC()
        sys.exit(app.exec())
    except Exception as e:
        QMessageBox.critical(None, "Критическая ошибка", f"Критическая ошибка: {e}")

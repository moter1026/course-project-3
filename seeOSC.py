import sys

import pyqtgraph as pg

from PyQt5 import QtWidgets
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QWidget, QPushButton,
                             QLabel, QVBoxLayout,
                             QHBoxLayout, QLineEdit,
                             QMainWindow, QFileDialog)

import Aegis_osc

from Fourier import Fourier
from work_with_osc import DataOsc


class MainMenu(QWidget):
    def __init__(self):
        super().__init__()  # Вызываю конструктор родительского класса QWidget
        self.dB_text = None
        self.text = None
        self.now_plot_spectr_item = None
        self.now_plot_osc_item = None
        self.main_layout_v = QVBoxLayout(self)

        # Кнопка для получения открытия отдельной осциллограммы
        self.bttn_open_alone_osc = QPushButton()
        self.bttn_open_alone_osc.setText("открыть осциллограмму")
        self.bttn_open_alone_osc.clicked.connect(self._on_clicked_bttn_open_alone_osc)

        self.file_open = QLabel(self)
        self.info_now_num_osc = QLabel(self)

        # Блок добавления элементов в главное меню
        self.main_layout_v.addWidget(self.bttn_open_alone_osc)

    def _on_clicked_bttn_open_alone_osc(self) -> None:
        """Обработчик события клика для кнопки создания нового датасета"""
        path, _ = QFileDialog.getOpenFileName(self, "Выберите файл", "", "OSC Files (*.osc)")

        self.__open_osc(path)

    def __open_osc(self, name_osc: str):
        try:
            self.file_open.setText(f"Открыт: {name_osc}")
            self.osc_file = Aegis_osc.File_osc(name_osc)
            self.num_osc = self.osc_file.m_sdoHdr.NumOSC
            self.osc_datas = []
            self.K_mkV = []
            self.dB_data = []
            self.start_data_osc = 0
            self.end_data_osc = 500 if self.num_osc > 500 else self.num_osc
            self.osc_datas.extend(self.osc_file.getDotsOSC(0, self.end_data_osc))

            for i in range(0, self.end_data_osc):
                self.K_mkV.append(self.osc_file.m_oscDefMod[i].K_mkV)
                self.dB_data.append(DataOsc.get_dB_osc(self.osc_datas[i], self.K_mkV[i]))

            self.plot_layout_h = QHBoxLayout(self)
            self.osc_now = 0
            if not hasattr(self, "now_plot_osc"):
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

            spectr = Fourier.four2(self.osc_datas[self.osc_now].copy())
            x = [i/(len(spectr)/2/500) for i in range(round(len(spectr)/2))]
            self.now_plot_spectr_item = self.now_plot_spectr.plot(x, spectr[:round(len(spectr)/2)])
            # Добавляем информацию о том, какя частота более выражена в сигнале
            index = round(spectr[:round(len(spectr)/2)].index(max(spectr[:round(len(spectr)/2)])))
            self.text = pg.TextItem(f"{x[index]} кГц", anchor=(0, 1), color="r")
            self.now_plot_spectr.addItem(self.text)

            # Связываем изменение диапазона графика с обновлением позиции текста
            self.now_plot_spectr.sigRangeChanged.connect(self.__update_info_position)

            # Добавляем информацию децибелах
            self.dB_text = pg.TextItem(f"{self.dB_data[self.osc_now]} Дб", anchor=(0, 1), color="r")
            self.now_plot_osc.addItem(self.dB_text)

            self.__update_info_position()

            # Связываем изменение диапазона графика с обновлением позиции текста
            self.now_plot_osc.sigRangeChanged.connect(self.__update_info_position)

            # self.plot_layout_h.addWidget(pg.plot(self.osc_datas[self.osc_now]))
            if (not hasattr(self, "bttn_next_osc") and
                    not hasattr(self, "bttn_prev_osc")):
                self.bttn_next_osc = QPushButton("Следующая осциллограмма")
                self.bttn_prev_osc = QPushButton("Предыдущая осциллограмма")
                self.bttn_next_osc.clicked.connect(self.open_next_osc)
                self.bttn_prev_osc.clicked.connect(self.open_prev_osc)
                self.main_layout_v.addWidget(self.bttn_next_osc)
                self.main_layout_v.addWidget(self.bttn_prev_osc)
            self.check_next_prev_osc()
        except Exception as e:
            print(f"Ошибка при открытии файла OSC: {e}")

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

    def __load_next_osc(self):
        if self.osc_now >= self.num_osc - 1:
            return
        self.end_data_osc = self.end_data_osc + 500 if (self.num_osc - self.osc_now) > 500 else self.num_osc - 1
        self.osc_datas.extend(self.osc_file.getDotsOSC(len(self.osc_datas)-1, self.end_data_osc))

        self.K_mkV.extend([self.osc_file.m_oscDefMod[i].K_mkV for i in range(len(self.osc_datas)-1, self.end_data_osc)])
        self.dB_data.extend([DataOsc.get_dB_osc(self.osc_datas[i], self.K_mkV[i])
                             for i in range(len(self.osc_datas)-1, self.end_data_osc)])

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

        self.now_plot_osc_item = self.now_plot_osc.plot(self.osc_datas[self.osc_now])
        spectr = Fourier.four2(self.osc_datas[self.osc_now].copy())
        x = [i / (len(spectr)/2/500) for i in range(round(len(spectr) / 2))]
        self.now_plot_spectr_item = self.now_plot_spectr.plot(x, spectr[:round(len(spectr) / 2)])

        # Добавляем информацию о том, какя частота более выражена в сигнале
        index = round(spectr[:round(len(spectr) / 2)].index(max(spectr[:round(len(spectr) / 2)])))
        self.text = pg.TextItem(f"{x[index]} кГц", anchor=(0, 1), color="r")
        self.now_plot_spectr.addItem(self.text)

        # Добавляем информацию децибелах
        self.dB_text = pg.TextItem(f"{self.dB_data[self.osc_now]} Дб", anchor=(0, 1), color="r")
        self.now_plot_osc.addItem(self.dB_text)

        self.__update_info_position()
        self.check_next_prev_osc()

    def open_prev_osc(self) -> None:
        if self.osc_now <= self.start_data_osc:
            self.check_next_prev_osc()
            return
        self.osc_now -= 1
        self.now_plot_osc.clear()
        self.now_plot_spectr.clear()

        self.info_now_num_osc.setText(f"Номер кадра: {self.osc_now + 1} из {self.num_osc}")

        self.now_plot_osc_item = self.now_plot_osc.plot(self.osc_datas[self.osc_now])
        spectr = Fourier.four2(self.osc_datas[self.osc_now].copy())
        x = [i / (len(spectr)/2/500) for i in range(round(len(spectr) / 2))]
        self.now_plot_spectr_item = self.now_plot_spectr.plot(x, spectr[:round(len(spectr) / 2)])

        # Добавляем текст (аннотацию)
        index = round(spectr[:round(len(spectr) / 2)].index(max(spectr[:round(len(spectr) / 2)])))
        self.text = pg.TextItem(f"{x[index]} кГц", anchor=(0, 1), color="r")
        self.now_plot_spectr.addItem(self.text)

        # Добавляем информацию децибелах
        self.dB_text = pg.TextItem(f"{self.dB_data[self.osc_now]} Дб", anchor=(0, 1), color="r")
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
        print(e)
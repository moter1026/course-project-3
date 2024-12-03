import os
import sys

from PyQt5 import QtWidgets
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import (QWidget, QPushButton,
                             QLabel, QVBoxLayout,
                             QHBoxLayout, QLineEdit,
                             QMainWindow, QFileDialog,
                             QMessageBox)
from PyQt5.QtCore import (QPropertyAnimation, QEvent, QObject)

import Aegis_osc


class HoverHandler(QObject):
    def eventFilter(self, obj, event):
        if isinstance(obj, QPushButton):
            if event.type() == QEvent.Enter:
                self.animate_button_colors(
                    obj,
                    start_bg=QColor("#0F0F0F"), end_bg=QColor("white"),
                    start_text=QColor("white"), end_text=QColor("#0F0F0F")
                )
            elif event.type() == QEvent.Leave:
                self.animate_button_colors(
                    obj,
                    start_bg=QColor("white"), end_bg=QColor("#0F0F0F"),
                    start_text=QColor("#0F0F0F"), end_text=QColor("white")
                )
        return super().eventFilter(obj, event)

    def animate_button_colors(self, button, start_bg, end_bg, start_text, end_text):
        """Анимация изменения цвета кнопки и текста."""
        # Анимация для фона
        bg_animation = QPropertyAnimation(button, b"bg_color")
        bg_animation.setDuration(200)
        bg_animation.setStartValue(start_bg)
        bg_animation.setEndValue(end_bg)

        # Анимация для текста
        text_animation = QPropertyAnimation(button, b"text_color")
        text_animation.setDuration(200)
        text_animation.setStartValue(start_text)
        text_animation.setEndValue(end_text)

        # Динамическое обновление стиля
        def update_button_style(bg_color, text_color):
            button.setStyleSheet(f"""
                QPushButton {{
                    background-color: {bg_color.name()};
                    color: {text_color.name()};
                    border: 1px solid gray;
                    border-radius: 5px;
                    padding: 5px;
                }}
            """)

        # Связываем анимации с обновлением стилей
        bg_animation.valueChanged.connect(lambda color: update_button_style(color, end_text))
        text_animation.valueChanged.connect(lambda color: update_button_style(end_bg, color))

        # Запускаем анимации
        bg_animation.start()
        text_animation.start()

        # Храним ссылки, чтобы Python не удалил анимации
        button._bg_animation = bg_animation
        button._text_animation = text_animation


class OscCateg(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setStyleSheet("""
                    QMainWindow {
                        min-width: 600px;
                        background-color: #EBFFFF;
                    }
                    QLineEdit {
                        border: 2px solid gray;
                        border-radius: 5px;
                        padding: 5px;
                        font-size: 14px;
                        background-color: rgba(0,255,4, 0.6);
                        transition: 0.2s;
                    }
                    QLineEdit:focus {
                        border: 1px solid #0F0F0F;
                    }
                    QPushButton {
                        color: white;
                        background-color: #0F0F0F;
                        border: 1px solid gray;
                        border-radius: 5px;
                        padding: 5px;
                    }
                """)

        self.main_widjet = QWidget(self)
        self.main_layout_v = QVBoxLayout(self.main_widjet)
        self.main_widjet.setLayout(self.main_layout_v)

        self.from_osc_layout_h = QHBoxLayout(self.main_widjet)
        self.start_number_layout_h = QHBoxLayout(self.main_widjet)
        self.end_number_layout_h = QHBoxLayout(self.main_widjet)
        self.to_osc_layout_h = QHBoxLayout(self.main_widjet)

        self.edit_from = QLineEdit(self.main_widjet)
        self.bttn_open_from_osc = QPushButton("Открыть осн. осциллограмму", self.main_widjet)
        self.bttn_open_from_osc.clicked.connect(self.__on_clicked_bttn_open_from_osc)

        self.edit_start_count = QLineEdit(self.main_widjet)
        self.edit_start_count.setFixedWidth(70)
        self.text_start_count = QLabel("начальная осциллограмма", self)

        self.edit_end_count = QLineEdit(self.main_widjet)
        self.edit_end_count.setFixedWidth(70)
        self.text_end_count = QLabel("конечная осциллограмма", self)

        self.edit_to = QLineEdit(self.main_widjet)
        self.bttn_open_to_osc = QPushButton("путь для записи нового .osc", self.main_widjet)
        self.bttn_open_to_osc.clicked.connect(self.__on_clicked_bttn_open_to_osc)

        self.bttn_go = QPushButton("Начать копирование", self.main_widjet)
        self.bttn_go.clicked.connect(self.__on_clicked_bttn_go)

        self.from_osc_layout_h.addWidget(self.edit_from)
        self.from_osc_layout_h.addWidget(self.bttn_open_from_osc)

        self.start_number_layout_h.addWidget(self.edit_start_count)
        self.start_number_layout_h.addWidget(self.text_start_count)

        self.end_number_layout_h.addWidget(self.edit_end_count)
        self.end_number_layout_h.addWidget(self.text_end_count)

        self.to_osc_layout_h.addWidget(self.edit_to)
        self.to_osc_layout_h.addWidget(self.bttn_open_to_osc)

        self.main_layout_v.addLayout(self.from_osc_layout_h)
        self.main_layout_v.addLayout(self.start_number_layout_h)
        self.main_layout_v.addLayout(self.end_number_layout_h)
        self.main_layout_v.addLayout(self.to_osc_layout_h)
        self.main_layout_v.addWidget(self.bttn_go)

        self.setCentralWidget(self.main_widjet)

        # Устанавливаем фильтр событий
        self.hover_handler = HoverHandler()
        self.bttn_open_from_osc.installEventFilter(self.hover_handler)
        self.bttn_open_to_osc.installEventFilter(self.hover_handler)
        self.show()

    def __get_path(self) -> str:
        path, _ = QFileDialog.getOpenFileName(self, "Выберите файл", "", "OSC Files (*.osc)")
        return path

    def __on_clicked_bttn_open_to_osc(self) -> None:
        """"""
        path, _ = QFileDialog.getSaveFileName(self, "Выберите файл", "", "OSC Files (*.osc)")
        self.edit_to.setText(path)

    def __on_clicked_bttn_open_from_osc(self) -> None:
        """"""
        self.edit_from.setText(self.__get_path())

    def __on_clicked_bttn_go(self) -> None:
        path_from = self.edit_from.text()
        path_to = self.edit_to.text()
        index_start = int(self.edit_start_count.text()) - 1
        index_end = int(self.edit_end_count.text())
        if index_start < 0 or index_end < 0:
            QMessageBox.critical(self, "Неверный номер осциллограммы"
                                 , f"Номера осциллограмм могут быть только больше нуля")

        file_from = Aegis_osc.File_osc(path_from)
        num_osc_from = file_from.m_sdoHdr.NumOSC
        osc_datas = []
        if index_end > num_osc_from:
            QMessageBox.critical(self, "Неверный размер"
                                 , f"максимальный возможный номер осциллограммы {num_osc_from}")
            return
        osc_datas.extend(file_from.getDotsOSC(index_start, index_end))

        if os.path.exists(path_to):
            response = QMessageBox.information(
                self,
                "Файл существует",
                f"Файл '{path_to}' уже существует\n\n"
                f"Вы хотите перезаписать файл?",
                QMessageBox.Yes | QMessageBox.No
            )
            if response == QMessageBox.No:
                return
        # file_to = Aegis_osc.File_osc(path_to)
        file_from.saveOSC(path_to, osc_datas, range(index_start, index_end, 1))


if __name__ == "__main__":
    try:
        app = QtWidgets.QApplication([])
        app.setStyle("Fusion")
        ex = OscCateg()
        sys.exit(app.exec())
    except Exception as e:
        print(e)

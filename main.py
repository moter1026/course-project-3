import codecs
import os
import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import (QWidget, QPushButton,
                             QLabel, QVBoxLayout,
                             QHBoxLayout, QLineEdit,
                             QMainWindow)

module_path = os.path.abspath("cpp/build/Debug")
sys.path.append(module_path)

# Теперь импортируем модуль
try:
    import Aegis_osc
    import numpy
    print("Модуль успешно импортирован.")
except ImportError as e:
    print(f"Ошибка импорта: {e}")



class Main_menu(QWidget):
    def __init__(self):
        super().__init__()  # Вызываю конструктор родительского класса QWidget
        self.main_layout_v = QVBoxLayout(self)

        self.label = QLabel()
        self.label.setText("автор Пихуров Матвей 6311-100503D")
        self.label.setStyleSheet("font-size: 18px;")

        self.main_layout_v.addWidget(self.label)

        



class My_app(QMainWindow):
    def __init__(self):
        super().__init__()
        self.create_menu()

    def create_menu(self):
        self.menu = Main_menu()
        
        self.setCentralWidget(self.menu)
        self.show()




if __name__ == "__main__":
    try:
        file = Aegis_osc.File_osc()
        file.readOsc(f"cpp/aem19_06_23#01.osc")
        print(file.m_measData.get_obj_name())
        print(file.m_measData.get_oper_name())
        print(f"\n")
        file.readOsc(f"cpp/aem19_06_23#03.osc")
        print(file.m_measData.get_obj_name())
        print(file.m_measData.get_oper_name())
        print(f"\n")
        file.readOsc(f"cpp/aem05_06_23#01.osc")
        print(file.m_measData.get_obj_name())
        print(file.m_measData.get_oper_name())
        print(f"\n")
        file.readOsc(f"cpp/aem08_06_23#01.osc")
        print(file.m_measData.get_obj_name())
        print(file.m_measData.get_oper_name())
        print(f"\n")
        file.readOsc(f"C:/Users/Матвей/Desktop/osc/Актубинский/aem19_06_23#03.osc")
        print(file.m_measData.get_obj_name())
        print(file.m_measData.get_oper_name())
        # print(file.m_measData.get_reference())

        app = QtWidgets.QApplication([])
        ex = My_app()
        sys.exit(app.exec())
    except Exception as e:
        print(e)

    
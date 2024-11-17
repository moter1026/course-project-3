import csv
import math
import os
import sys
import matplotlib.pyplot as plt
import numpy as np
from tensorflow.keras.models import load_model
from scipy.fft import fft, ifft
from PyQt5.QtWidgets import QFileDialog

module_path = os.path.abspath("cpp/build/Debug")
sys.path.append(module_path)
try:
    import Aegis_osc
except ImportError as e:
    raise "Не удалось импортировать модуль для работы с осциллограммами!"


class TestModel():
    def __init__(self):
        # Загрузка модели
        self.model = load_model('NO BAD MODEL.keras')
        self.padded_data_oscs = []
        self.data_list = []
        self.max_length = 4000
        self.file_osc = None
        self.np_data_oscs = None
        self.np_spectr_list = None
        self.np_features_list = None
        self.categories = {0: "Best", 1: "Good", 2: "Check", 3: "Noise", 4: "Vibration"}

    def __load_osc(self, name_osc: str) -> list:
        """получает информацию о файле осциллограммы"""
        self.file_osc = Aegis_osc.File_osc(name_osc)
        self.num_osc = self.file_osc.m_sdoHdr.NumOSC
        return self.data_list

    def __get_data_for_predict(self) -> list:
        # features_list = []
        smoothed_signal = []
        spectr_list = []
        for signal in self.data_list:
            current_length = len(signal)

            # Получаем значения из мат. статистики, которые могут помочь
            # в обучении нейронной сети
            # mean_signal = np.mean(signal)
            # std_dev_signal = np.std(signal)
            # variance_signal = np.var(signal)
            # kurtosis_signal = np.mean((signal - mean_signal) ** 4) / (std_dev_signal ** 4)
            # min_val_signal = np.min(signal)
            # max_val_signal = np.max(signal)
            # energy_signal = np.sum([i ** 2 for i in signal])
            # features_list.append([mean_signal, std_dev_signal, variance_signal,
            #                       kurtosis_signal, min_val_signal, max_val_signal,
            #                       energy_signal])

            # Если сигнал уже нужной длины, возвращаем его
            if current_length >= self.max_length:
                smoothed_signal.append(signal)
                # Получаем спектр и запоминаем его
                spectrum = fft(signal.copy())
                spectrum_length = len(spectrum)
                spectr_list.append(spectrum)
                continue

            # Инициализация дополненного массива
            padded_signal = np.zeros(self.max_length)
            padded_signal[:current_length] = signal.copy()

            # Генерируем шум на основе статистики сигнала
            mean = np.mean(signal[round(len(signal) * 0.7):])
            std = np.std(signal[round(len(signal) * 0.7):])
            noise = np.random.normal(loc=mean, scale=std, size=self.max_length - current_length)

            # Добавляем шум к оставшейся части массива
            padded_signal[current_length:] = noise

            # Переходим в частотную область, чтобы сгладить переход
            # spectrum_padded = fft(padded_signal)

            # Применяем обратное преобразование Фурье для получения дополненного сигнала
            # from_spectr = np.real(ifft(spectrum_padded))
            # from_spectr[:current_length] = signal

            # Получаем спектр после приведения к нужной длине и сохраняем его
            spectrum = fft(padded_signal)
            spectr_list.append(spectrum)

            smoothed_signal.append(padded_signal)

        self.np_data_oscs = np.array(smoothed_signal).astype(np.int16)
        self.np_data_oscs = np.expand_dims(self.np_data_oscs, axis=-1)

        self.np_spectr_list = np.array(spectr_list)
        self.np_spectr_list = np.real(self.np_spectr_list).astype(np.int16)

        # self.np_features_list = np.array(features_list)
        return [self.np_data_oscs, self.np_spectr_list]

    @staticmethod
    def max_in_ind(arr: np.ndarray):
        maximum = arr[0]
        ind = 0
        for i, val in enumerate(arr):
            if val > maximum:
                maximum = val
                ind = i
        return ind, maximum

    def predict(self, file_osc) -> dict:
        """Разбивает осциллограммы на категории """
        self.__load_osc(file_osc)
        predictions = []
        if self.num_osc > 10000:
            iter = self.num_osc / 10000
            start = 0
            end = 10000
            for _ in range(math.ceil(iter)):
                self.data_list = self.file_osc.getDotsOSC(start, end)
                datas = self.__get_data_for_predict()
                predictions.extend(self.model.predict(datas))
                iter -= 1
                start = end
                end = start + 10000 if iter > 1 else round(start + (iter * 10000))
        else:
            self.data_list = self.file_osc.getDotsOSC(0, self.file_osc.m_sdoHdr.NumOSC)
            datas = self.__get_data_for_predict()
            predictions.extend(self.model.predict(datas))

        return self.__split_into_catregories([self.max_in_ind(arrays) for arrays in predictions])

    def __split_into_catregories(self, response: list) -> dict:
        res = {"Best": [],
               "Good": [],
               "Check": [],
               "Noise": [],
               "Vibration": []}
        for ind, arr in enumerate(response):
            if self.categories[arr[0]] == "Best":
                res["Best"].append(ind)
            elif self.categories[arr[0]] == "Good":
                res["Good"].append(ind)
            elif self.categories[arr[0]] == "Check":
                res["Check"].append(ind)
            elif self.categories[arr[0]] == "Noise":
                res["Noise"].append(ind)
            elif self.categories[arr[0]] == "Vibration":
                res["Vibration"].append(ind)
        return res


import inspect
import math
import logging
import sys
import os

from typing import Any

import numpy as np

from numba import njit
from multiprocessing import Pool, cpu_count

from tensorflow.keras.models import load_model

import Aegis_osc

from Fourier import four2
from work_with_osc import get_dB_osc


# Отключение вывода лишних сообщений от TensorFlow
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
LOG_LEVEL = Aegis_osc.LogLevel


def process_chunk(args):
    """Обрабатывает кусок данных осциллограмм."""
    print("in proccess_chunck")
    data_list, K_mkV_list, max_length, get_data_for_predict, model = args
    datas = get_data_for_predict(data_list, K_mkV_list, max_length)
    print("get datas")
    predictions = model.predict(datas, verbose=0)
    return predictions


@njit
def get_data_for_predict(data_list: np.ndarray, K_mkV_list: np.ndarray, max_length: np.int32) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """
    Метод возвращает данные, необходимые для предсказани категорий осциллограмм.
    Возвращает:
        1) np.ndarray np_data_oscs - список осциллограмм
        2) np.ndarray np_spectr_list - список спектров
        3) np.ndarray np_dB_list - список децибел для осциллограмм
    """
    smoothed_signal = np.zeros((len(data_list), max_length), dtype=np.float64)  # Массив с нужной длиной
    spectr_list = np.zeros((len(data_list), 4096), dtype=np.float64)  # Массив с нужной длиной
    dB_list = np.zeros(len(data_list), dtype=np.int32)  # Массив с нужной длиной

    for i, signal in enumerate(data_list):
        current_length = len(signal)

        # Если сигнал уже нужной длины, возвращаем его
        if current_length >= max_length:
            smoothed_signal[i] = signal
            # Получаем спектр и запоминаем его
            spectr = four2(signal)
            spectrum = np.array([i / (len(spectr) / 2 / 500) for i in range(round(len(spectr) / 2))])
            # spectr_list[i] = spectrum[:max_length]
            spectr_list[i] = spectrum
            dB_list[i] = get_dB_osc(signal, K_mkV_list[i])
            continue

        # Инициализация дополненного массива
        padded_signal = np.zeros(max_length)
        padded_signal[:current_length] = signal.copy()

        # Генерируем шум на основе статистики сигнала
        mean = np.mean(signal[round(len(signal) * 0.7):])
        std = np.std(signal[round(len(signal) * 0.7):])
        noise = np.random.normal(loc=mean, scale=std, size=max_length - current_length)

        # Добавляем шум к оставшейся части массива
        padded_signal[current_length:] = noise

        # Получаем спектр после приведения к нужной длине и сохраняем его
        spectr = four2(padded_signal)
        spectrum = np.array([i / (len(spectr) / 2 / 500) for i in range(round(len(spectr) / 2))])
        # spectr_list[i] = spectrum[:max_length]
        spectr_list[i] = spectrum

        dB_list[i] = get_dB_osc(signal, K_mkV_list[i])

        smoothed_signal[i] = padded_signal

    np_data_oscs = smoothed_signal
    np_data_oscs = np_data_oscs.reshape((len(np_data_oscs), len(np_data_oscs[i]), 1))

    return np_data_oscs, spectr_list, dB_list


class TestModel:
    def __init__(self):
        # Загрузка модели
        self.K_mkV_list = None
        self.model = load_model('best_model.keras')
        self.padded_data_oscs = []
        self.data_list = []
        self.max_length = np.int32(4000)
        self.file_osc = None
        self.np_data_oscs = None
        self.np_spectr_list = None
        self.np_features_list = None
        self.categories = {0: "Best", 1: "Good", 2: "Check", 3: "Noise", 4: "Vibration"}

    def __load_osc(self, name_osc: str, logger: Aegis_osc.Logger) -> list:
        """получает информацию о файле осциллограммы"""
        self.file_osc = Aegis_osc.File_osc(name_osc, logger)
        self.num_osc = self.file_osc.sdoHdr.NumOSC
        return self.data_list

    @staticmethod
    def max_in_ind(arr: np.ndarray):
        maximum = arr[0]
        ind = 0
        for i, val in enumerate(arr):
            if val > maximum:
                maximum = val
                ind = i
        return ind, maximum

    @staticmethod
    def max_in_ind(arr: np.ndarray):
        """
        Оптимизированная функция для поиска индекса и значения максимума.
        """
        ind = np.argmax(arr)
        return ind, arr[ind]

    def predict(self, file_osc: str, logger: Aegis_osc.Logger) -> dict:
        """Разбивает осциллограммы на категории """
        self.__load_osc(file_osc, logger)
        predictions = []
        logger.logg(LOG_LEVEL._INFO_, "start predict",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)

        if self.num_osc > 1000:
            logger.logg(LOG_LEVEL._INFO_, "predict with multiprocessing",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            chunk_size = 1000
            num_chunks = math.ceil(self.num_osc / chunk_size)
            chunk_args = []
            logger.logg(LOG_LEVEL._INFO_, "start get chunk_args",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            for i in range(num_chunks):
                start, end = (i * chunk_size, min((i + 1) * chunk_size, self.num_osc))  # Передаём диапазон
                self.data_list = np.array(self.file_osc.getDotsOSC(start, end))
                self.K_mkV_list = self.file_osc.get_K_mkV(start, end)
                chunk_args.append((self.data_list, self.K_mkV_list, self.max_length, get_data_for_predict, self.model))
            logger.logg(LOG_LEVEL._INFO_, "end get chunk_args",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            # Параллельная обработка с использованием пула процессов
            logger.logg(LOG_LEVEL._INFO_, "start multiprocessing",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            with Pool(processes=cpu_count()) as pool:
                results = pool.map(process_chunk, chunk_args)
            logger.logg(LOG_LEVEL._INFO_, "end multiprocessing",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            # Объединяем результаты всех процессов
            for result in results:
                predictions.extend(result)

        else:
            logger.logg(LOG_LEVEL._INFO_, "predict without multiprocessing",
                        os.path.basename(__file__),
                        inspect.currentframe().f_lineno, 
                        inspect.currentframe().f_code.co_name)
            self.data_list = np.array(self.file_osc.getDotsOSC(0, self.file_osc.sdoHdr.NumOSC))
            self.K_mkV_list = np.array([self.file_osc.oscDefMod[i].K_mkV for i in range(0, self.file_osc.sdoHdr.NumOSC)])
            datas = get_data_for_predict(self.data_list, self.K_mkV_list, self.max_length)
            predictions.extend(self.model.predict(datas, verbose=0))

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

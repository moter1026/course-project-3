import csv
import math
import os
import sys
import matplotlib.pyplot as plt
import numpy as np
from tensorflow.keras.models import load_model

module_path = os.path.abspath("cpp/build/Debug")
sys.path.append(module_path)
try:
    import Aegis_osc
except ImportError as e:
    raise "Не удалось импортировать модуль для работы с осциллограммами!"


def max_in_ind(arr: np.ndarray):
    maximum = arr[0]
    ind = 0
    for i, val in enumerate(arr):
        if val > maximum:
            maximum = val
            ind = i
    return ind, maximum


# Загрузка модели
model = load_model('my_model_24_12_11_corrected_dataset_30epochs.keras')

name_osc = "C:/Users/Матвей/Desktop/osc/Татнефть-Добыча/aem05_06_23#03.osc"
file_osc = Aegis_osc.File_osc(name_osc)
data_list = file_osc.getDotsOSC(0, file_osc.m_sdoHdr.NumOSC)

# h_z = math.ceil(1 + 3.322 * math.log10(len(data_list[1])) + 3)
# #полученные интервалы
# bins = (np.arange(min(data_list[0]), max(data_list[0]) , (max(data_list[0]) - min(data_list[0]))/h_z))
# plt.hist(data_list[1], len(bins), density=True, alpha=0.6, color='g', edgecolor='black')
# plt.show()

padded_data_oscs = []
length = 4000
for sublist in data_list:
    mean = np.mean(sublist)
    std = np.std(sublist)
    padded_data_oscs.append(sublist)
    # if len(sublist) < max_length:
    padded_data_oscs[-1].extend(np.random.normal(mean, std, length - len(sublist)).tolist())

# Например, X_new — это данные, которые вы хотите классифицировать
X_new = np.array(padded_data_oscs)  # Ваши новые данные

# Приводим X_new к нужной форме (если необходимо)
# X_new = X_new[..., np.newaxis]  # Если данные требуют добавления дополнительной размерности для Conv1D

# Делаем предсказание
predictions = model.predict(X_new)

response = [max_in_ind(arrays) for arrays in predictions]

# Для многоклассовой классификации, выводим вероятности для каждого класса
with open(name_osc[:name_osc.rfind(".")]+".txt", "w+", encoding="utf-16") as csv_reader:
    writer = csv.writer(csv_reader, delimiter=";")
    data = []
    for i, arr in enumerate(response):
        writer.writerow([i, arr[0], arr[1]])

print(response)

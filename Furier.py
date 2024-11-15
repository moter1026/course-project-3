import os
import sys

import numpy as np
# import matplotlib.pyplot as plt

# Теперь импортируем модуль для работы с osc
module_path = os.path.abspath("cpp/build/Debug")
sys.path.append(module_path)
try:
    import Aegis_osc
except ImportError as e:
    raise "Не удалось импортировать модуль для работы с осциллограммами!"


def find_closest_power(number):
    """
    Находит ближайшую степень двойки для заданного числа.

    Параметры:
    number - целое число

    Возвращает:
    power - ближайшая степень двойки (в виде целого числа)
    """
    power = 0
    number -= 1
    while number > 0:
        number >>= 1
        power += 1
    return power


def four2(x, M, D=-1):
    """
    Реализация алгоритма БПФ на Python на основе кода из книги В.П. Дьяконова.

    Параметры:
    x - массив входных данных (вещественная часть)
    M - степень двойки (N = 2^M)
    D - направление преобразования (-1 для прямого БПФ, 1 для обратного БПФ)
    """
    N = 1 << M  # N = 2^M
    y = np.zeros(N)  # массив для мнимой части

    # Прямое или обратное БПФ
    for l in range(1, M + 1):
        e = 1 << (M + 1 - l)
        f = e >> 1
        u = 1.0
        v = 0.0

        z = np.pi / f
        c = np.cos(z)
        s = D * np.sin(z)

        for j in range(1, f + 1):
            for i in range(j, N + 1, e):
                o = i + f - 1
                p = x[i - 1] + x[o]
                q = y[i - 1] + y[o]  # мнимая часть
                r = x[i - 1] - x[o]
                t = y[i - 1] - y[o]  # мнимая часть
                x[o] = r * u - t * v
                y[o] = t * u + r * v
                x[i - 1] = p
                y[i - 1] = q

            w = u * c - v * s
            v = v * c + u * s
            u = w

    # Перестановка элементов (битовая инверсия)
    j = 1
    for i in range(1, N):
        if i < j:
            j1 = j - 1
            i1 = i - 1
            # Обмен вещественной и мнимой частей
            x[j1], x[i1] = x[i1], x[j1]
            y[j1], y[i1] = y[i1], y[j1]
        k = N >> 1
        while k < j:
            j -= k
            k >>= 1
        j += k

    # Прямое или обратное БПФ
    if D < 0:  # прямое БПФ
        for k in range(N):
            a = np.sqrt(x[k] ** 2 + y[k] ** 2)
            x[k] = a * 2.0 / N
    else:  # обратное БПФ
        for k in range(N):
            x[k] /= N
            y[k] /= N

    return x


def abs_values_of_spectr(file_osc: Aegis_osc.File_osc, num_osc: int) -> tuple[list, list]:
    buf_size_max = max(2048, (1 << (find_closest_power(file_osc.m_oscDefMod[num_osc].buf_size_max - 1))))
    spectr_buf_size = 1 << (find_closest_power(file_osc.m_oscDefMod[num_osc].buf_size) - 1)
    # interval = file_osc.m_oscDefMod[num_osc].freq / 25 * buf_size_max / spectr_buf_size

    osc = file_osc.getDotOSC(6)
    length_osc = len(osc)
    M = find_closest_power(length_osc)
    osc.extend([0 for _ in range((2 << M - 1) - length_osc)])

    # Выполнение прямого БПФ
    spectra = four2(osc.copy(), M, D=-1)
    spectra[0] = 0

    K_mkV = file_osc.m_oscDefMod[num_osc].K_mkV
    freq = file_osc.m_oscDefMod[num_osc].freq
    x = []
    y = []
    for i in range(spectr_buf_size):
        # x.append(round(i * interval))
        x.append(i/(spectr_buf_size / 500))
        y.append(round(spectra[i] * K_mkV / freq * 2896309))
    return x, y


# osc_file = Aegis_osc.File_osc("cpp/aem08_06_23#01.osc")
#
# x, y = abs_values_of_spectr(osc_file, 6)
# plt.plot(x, y)
#
# plt.show()

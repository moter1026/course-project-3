import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from tensorflow.keras.callbacks import EarlyStopping

# Загрузка данных
input_signal = np.load("description_24.12.02 ОСЦ Для Матвея_copy_values.npy")  # Осциллограммы
input_signal = np.expand_dims(input_signal, axis=-1)  # Преобразуем (batch_size, seq_len) в (batch_size, seq_len, 1)
input_signal = (input_signal - np.mean(input_signal)) / np.std(input_signal)  # Нормализация сигнала

input_spectrum = np.load("description_24.12.02 ОСЦ Для Матвея_copy_spectr.npy")  # Спектр
input_spectrum = np.real(input_spectrum)  # Удаляем мнимую часть
input_spectrum = (input_spectrum - np.mean(input_spectrum)) / np.std(input_spectrum)  # Нормализация спектра

input_dB = np.load("description_24.12.02 ОСЦ Для Матвея_copy_dB.npy")  # Децибеллы
input_dB = np.expand_dims(input_dB, axis=-1)  # Преобразуем (batch_size,) в (batch_size, 1)
input_dB = (input_dB - np.mean(input_dB)) / np.std(input_dB)  # Нормализация dB

# Убедимся, что все входные данные имеют одинаковый размер batch_size
assert input_signal.shape[0] == input_spectrum.shape[0] == input_dB.shape[0], \
    "Размеры batch_size для input_signal, input_spectrum и input_dB не совпадают"

# Инициализация кодировщика меток
classes_signal = np.load("description_24.12.02 ОСЦ Для Матвея_copy_categories.npy")  # Метки
label_encoder = LabelEncoder()
classes_signal = label_encoder.fit_transform(classes_signal)

# Определение входов для модели
input_signal_keras = layers.Input(shape=(input_signal.shape[1], 1), name='input_signal')
input_spectrum_keras = layers.Input(shape=(input_spectrum.shape[1],), name='input_spectrum')
input_dB_keras = layers.Input(shape=(1,), name='input_dB')  # Форма (batch_size, 1)

# Обработка временного сигнала с помощью 1D-CNN
x1 = layers.Conv1D(64, kernel_size=3, activation='relu', kernel_regularizer='l2')(input_signal_keras)
x1 = layers.MaxPooling1D(pool_size=2)(x1)
x1 = layers.Conv1D(128, kernel_size=3, activation='relu', kernel_regularizer='l2')(x1)
x1 = layers.GlobalAveragePooling1D()(x1)

# Обработка спектральных данных с помощью Dense слоев
x2 = layers.Dense(64, activation='relu', kernel_regularizer='l2')(input_spectrum_keras)
x2 = layers.Dense(32, activation='relu', kernel_regularizer='l2')(x2)

# Обработка dB данных
x4 = layers.Dense(32, activation='relu', kernel_regularizer='l2')(input_dB_keras)

# Объединение всех входных потоков
combined = layers.concatenate([x1, x2, x4])

# Финальные Dense слои для классификации
x = layers.Dense(64, activation='relu', kernel_regularizer='l2')(combined)
x = layers.Dropout(0.5)(x)
output = layers.Dense(len(np.unique(classes_signal)), activation='softmax')(x)

# Создание модели
model = models.Model(inputs=[input_signal_keras, input_spectrum_keras, input_dB_keras], outputs=output)

# Компиляция модели с корректной функцией потерь
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

# Разделение данных на обучающую и тестовую выборки
X_train_sig, X_test_sig, X_train_spec, X_test_spec, X_train_dB, X_test_dB, y_train, y_test = train_test_split(
    input_signal, input_spectrum, input_dB, classes_signal, test_size=0.2, random_state=42
)

# Настройка ранней остановки
early_stopping = EarlyStopping(monitor='val_loss', patience=5, restore_best_weights=True)

# Обучение модели
epochs = 20
history = model.fit(
    [X_train_sig, X_train_spec, X_train_dB], y_train,
    validation_data=([X_test_sig, X_test_spec, X_test_dB], y_test),
    epochs=epochs,
    batch_size=64,
    callbacks=[early_stopping]
)

# Сохранение модели
model.save(f"my_model_24_12_03_{epochs}_epochs.keras")

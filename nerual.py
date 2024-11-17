import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder

# Загрузка данных
input_signal = np.load("description_24.12.11 ОСЦ Для Матвея_copy_values.npy")  # Осциллограммы
input_signal = np.expand_dims(input_signal, axis=-1)  # Преобразуем (batch_size, seq_len) в (batch_size, seq_len, 1)

input_spectrum = np.load("description_24.12.11 ОСЦ Для Матвея_copy_spectr.npy")  # Спектр
input_spectrum = np.real(input_spectrum)

# input_features = np.load("description_24.12.11 ОСЦ Для Матвея_copy_features.npy")  # Мат.Стат. признаки

classes_signal = np.load("description_24.12.11 ОСЦ Для Матвея_copy_categories.npy")  # Метки (сигнал, шум и т.д.)

# Инициализация кодировщика меток
label_encoder = LabelEncoder()
classes_signal = label_encoder.fit_transform(classes_signal)

# Определение входов для модели
input_signal_keras = layers.Input(shape=(input_signal.shape[1], 1), name='input_signal')
input_spectrum_keras = layers.Input(shape=(input_spectrum.shape[1],), name='input_spectrum')
# input_features_keras = layers.Input(shape=(input_features.shape[1],), name='input_features')

# Обработка временного сигнала с помощью 1D-CNN
x1 = layers.Conv1D(64, kernel_size=3, activation='relu')(input_signal_keras)
x1 = layers.MaxPooling1D(pool_size=2)(x1)
x1 = layers.Conv1D(128, kernel_size=3, activation='relu')(x1)
x1 = layers.GlobalAveragePooling1D()(x1)

# Обработка спектральных данных с помощью Dense слоев
x2 = layers.Dense(64, activation='relu')(input_spectrum_keras)
x2 = layers.Dense(32, activation='relu')(x2)

# Обработка дополнительных признаков
# x3 = layers.Dense(32, activation='relu')(input_features_keras)

# Объединение всех входных потоков
# combined = layers.concatenate([x1, x2, x3])
combined = layers.concatenate([x1, x2])

# Финальные Dense слои для классификации
x = layers.Dense(64, activation='relu')(combined)
output = layers.Dense(len(np.unique(classes_signal)), activation='softmax')(x)

# Создание модели
# model = models.Model(inputs=[input_signal_keras, input_spectrum_keras, input_features_keras], outputs=output)
model = models.Model(inputs=[input_signal_keras, input_spectrum_keras], outputs=output)

# Компиляция модели
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

# Разделение данных на обучающую и тестовую выборки
(X_train_sig, X_test_sig,
 X_train_spec, X_test_spec,
 # X_train_features, X_test_features,
 y_train, y_test) = train_test_split(input_signal, input_spectrum,
                                     classes_signal, test_size=0.2, random_state=42)

# Обучение модели
epochs = 30
# model.fit(
#     [X_train_sig, X_train_spec, X_train_features], y_train,
#     validation_data=([X_test_sig, X_test_spec, X_test_features], y_test),
#     epochs=30,
#     batch_size=32
# )
model.fit(
    [X_train_sig, X_train_spec], y_train,
    validation_data=([X_test_sig, X_test_spec], y_test),
    epochs=20,
    batch_size=32
)
model.save(f"my_model2_24_12_11_corrected_dataset_{epochs}epochs.keras")

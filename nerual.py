import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder

# Допустим, ваши данные находятся в формате numpy arrays
X = np.load("description_24.12.11 ОСЦ Для Матвея_copy_values.npy")  # Осциллограммы
y = np.load("description_24.12.11 ОСЦ Для Матвея_copy_categories.npy")   # Метки (сигнал, шум, вибрация и т.п.)

# Инициализируем кодировщик
label_encoder = LabelEncoder()
y = label_encoder.fit_transform(y)


# Разделение на обучающую и тестовую выборки
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)


# Построение модели CNN
model = models.Sequential([
    layers.Conv1D(64, kernel_size=3, activation='relu', input_shape=(X_train.shape[1], 1)),
    layers.MaxPooling1D(2),
    layers.Conv1D(128, kernel_size=3, activation='relu'),
    layers.MaxPooling1D(2),
    layers.Flatten(),
    layers.Dense(64, activation='relu'),
    layers.Dense(len(np.unique(y)), activation='softmax')
])

model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
epochs = 20
model.fit(X_train, y_train, epochs=epochs, validation_data=(X_test, y_test))
model.save(f"my_model_24_12_11_corrected_dataset_{epochs}epochs.keras")

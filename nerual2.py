import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from tensorflow.keras.callbacks import EarlyStopping, LearningRateScheduler, ReduceLROnPlateau
from tensorflow.keras.metrics import Precision, Recall
from tensorflow.keras.utils import to_categorical

# Загрузка данных
input_signal = np.load("description_24.12.20 ОСЦ ДЛЯ МАТВЕЯ ровно_copy_values.npy")  # Осциллограммы
input_signal = np.expand_dims(input_signal, axis=-1)  # Преобразуем (batch_size, seq_len) в (batch_size, seq_len, 1)
input_signal = (input_signal - np.mean(input_signal)) / np.std(input_signal)  # Нормализация сигнала

input_spectrum = np.load("description_24.12.20 ОСЦ ДЛЯ МАТВЕЯ ровно_copy_spectr.npy")  # Спектр
input_spectrum = np.real(input_spectrum)  # Удаляем мнимую часть
input_spectrum = (input_spectrum - np.mean(input_spectrum)) / np.std(input_spectrum)  # Нормализация спектра

input_dB = np.load("description_24.12.20 ОСЦ ДЛЯ МАТВЕЯ ровно_copy_dB.npy")  # Децибеллы
input_dB = np.expand_dims(input_dB, axis=-1)  # Преобразуем (batch_size,) в (batch_size, 1)
input_dB = (input_dB - np.mean(input_dB)) / np.std(input_dB)  # Нормализация dB

# Убедимся, что все входные данные имеют одинаковый размер batch_size
assert input_signal.shape[0] == input_spectrum.shape[0] == input_dB.shape[0], \
    "Размеры batch_size для input_signal, input_spectrum и input_dB не совпадают"

# Инициализация кодировщика меток
classes_signal = np.load("description_24.12.20 ОСЦ ДЛЯ МАТВЕЯ ровно_copy_categories.npy")  # Метки
label_encoder = LabelEncoder()
classes_signal = label_encoder.fit_transform(classes_signal)

# Define model inputs
input_signal_keras = layers.Input(shape=(input_signal.shape[1], 1), name='input_signal')
input_spectrum_keras = layers.Input(shape=(input_spectrum.shape[1],), name='input_spectrum')
input_dB_keras = layers.Input(shape=(1,), name='input_dB')

# Signal processing branch
x1 = layers.Conv1D(64, kernel_size=8, strides=4, padding='same')(input_signal_keras)
x1 = layers.BatchNormalization()(x1)
x1 = layers.Activation('relu')(x1)
x1 = layers.MaxPooling1D(pool_size=4)(x1)

x1 = layers.Conv1D(128, kernel_size=8, strides=4, padding='same')(x1)
x1 = layers.BatchNormalization()(x1)
x1 = layers.Activation('relu')(x1)
x1 = layers.MaxPooling1D(pool_size=4)(x1)

x1 = layers.GlobalAveragePooling1D()(x1)
x1 = layers.Dropout(0.3)(x1)

# Spectrum processing branch
x2 = layers.Reshape((input_spectrum.shape[1], 1))(input_spectrum_keras)
x2 = layers.Conv1D(64, kernel_size=8, strides=4, padding='same')(x2)
x2 = layers.BatchNormalization()(x2)
x2 = layers.Activation('relu')(x2)
x2 = layers.MaxPooling1D(pool_size=4)(x2)

x2 = layers.Conv1D(128, kernel_size=8, strides=4, padding='same')(x2)
x2 = layers.BatchNormalization()(x2)
x2 = layers.Activation('relu')(x2)
x2 = layers.GlobalAveragePooling1D()(x2)
x2 = layers.Dropout(0.3)(x2)

# dB processing branch
x3 = layers.Dense(32, activation='relu')(input_dB_keras)
x3 = layers.BatchNormalization()(x3)
x3 = layers.Dropout(0.2)(x3)

# Combine all features
combined = layers.concatenate([x1, x2, x3])

# Classification layers
x = layers.Dense(256, activation='relu')(combined)
x = layers.BatchNormalization()(x)
x = layers.Dropout(0.4)(x)
x = layers.Dense(128, activation='relu')(x)
x = layers.BatchNormalization()(x)
x = layers.Dropout(0.4)(x)
output = layers.Dense(5, activation='softmax')(x)  # 5 categories

# Create and compile model
model = models.Model(inputs=[input_signal_keras, input_spectrum_keras, input_dB_keras], 
                    outputs=output)

model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),
             loss='categorical_crossentropy',
             metrics=['accuracy', Precision(), Recall()])


# Добавляем LR Scheduler
def scheduler(epoch, lr):
    if epoch < 10:
        return lr
    else:
        return lr * 0.5  # Уменьшение learning rate после 10-й эпохи


lr_scheduler = LearningRateScheduler(scheduler)
reduce_lr = ReduceLROnPlateau(monitor='val_loss', factor=0.5, patience=3, min_lr=1e-6)


# Разделение данных на обучающую и тестовую выборки
X_train_sig, X_test_sig, X_train_spec, X_test_spec, X_train_dB, X_test_dB, y_train, y_test = train_test_split(
    input_signal, input_spectrum, input_dB, classes_signal, test_size=0.2, random_state=42
)


# Преобразуем метки в one-hot
y_train = to_categorical(y_train, num_classes=len(np.unique(classes_signal)))
y_test = to_categorical(y_test, num_classes=len(np.unique(classes_signal)))

# Изменяем loss-функцию на categorical_crossentropy
initial_learning_rate = 0.0005  # Было 0.001
model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=initial_learning_rate),
              loss='categorical_crossentropy',
              metrics=['accuracy', Precision(), Recall()])


# # Компиляция модели
# model.compile(optimizer='adam',
#               loss='sparse_categorical_crossentropy',
#               metrics=['accuracy', Precision(), Recall()])
# # Компиляция модели с корректной функцией потерь
# model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])


# callbacks = [
#     EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True),
#     ReduceLROnPlateau(monitor='val_loss', factor=0.5, patience=5, min_lr=1e-6),
# ]

# Обучение модели
epochs = 100
batch_size = 32
history = model.fit(
    [X_train_sig, X_train_spec, X_train_dB],
    y_train,
    batch_size=batch_size,
    epochs=epochs,
    validation_split=0.2,
    # callbacks=callbacks
)

# Сохранение модели
model.save(f"model_24.12.24 description_24.12.20 ОСЦ ДЛЯ МАТВЕЯ ровно_copy_withotEarlyStop_{epochs}-epochs_{batch_size}-batchsize.keras")


# Графики обучения
def plot_history(history):
    plt.figure(figsize=(12, 5))

    # Точность
    plt.subplot(1, 2, 1)
    plt.plot(history.history['accuracy'], label='Train Accuracy')
    plt.plot(history.history['val_accuracy'], label='Validation Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.legend()
    plt.title('Accuracy over Epochs')

    # Потери
    plt.subplot(1, 2, 2)
    plt.plot(history.history['loss'], label='Train Loss')
    plt.plot(history.history['val_loss'], label='Validation Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.title('Loss over Epochs')

    plt.show()


plot_history(history)

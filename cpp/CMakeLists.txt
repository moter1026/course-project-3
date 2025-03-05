# Устанавливаем минимальную версию CMake и имя проекта
cmake_minimum_required(VERSION 3.11)
project(Aegis_osc_project)

# Добавляем флаг для увеличения допустимого количества секций в объектном файле
add_compile_options(/bigobj)

# Устанавливаем политику CMP0148 для использования FindPython
if(POLICY CMP0148)
    cmake_policy(SET CMP0148 NEW)
endif()

# Явно указываем целевую архитектуру
#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#    add_compile_definitions(_WIN64)  # Для 64-битной сборки
#    message(STATUS "Target: 64-bit")
#else()
#    add_compile_definitions(_WIN32)  # Для 32-битной сборки
#    message(STATUS "Target: 32-bit")
#endif()

# Указываем путь к pybind11 вручную
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../.venv/Lib/site-packages/pybind11/share/cmake/pybind11")

# Устанавливаем переменную для поиска Python через FindPython
set(PYBIND11_FINDPYTHON ON)

# Подключаем директории для include-файлов
include_directories(${CMAKE_SOURCE_DIR}/gl)
include_directories(${CMAKE_SOURCE_DIR}/cpp)
include_directories(${CMAKE_SOURCE_DIR}/libraries)

# Включаем динамическую линковку MFC
add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2) # Использование MFC в DLL (динамическая линковка)

# Используем pybind11
find_package(pybind11 REQUIRED)

# Ищем все файлы .cpp в папке ./cpp
file(GLOB_RECURSE CPP_SOURCES "${CMAKE_SOURCE_DIR}/cpp/*.cpp")

# Выводим список файлов (для отладки)
message(STATUS "Found source files: ${CPP_SOURCES}")

# Создаем библиотеку с использованием найденных файлов
add_library(file_osc_module MODULE ${CPP_SOURCES})

# Линкуем библиотеку с pybind11
target_link_libraries(file_osc_module PRIVATE pybind11::module)

# Указываем версию Windows API
#target_compile_definitions(file_osc_module PRIVATE 
#    WINVER=0x0A00 
#    _WIN32_WINNT=0x0A00
#)

# Добавляем статическую линковку для всех стандартных библиотек
target_link_libraries(file_osc_module PRIVATE
    ws2_32.lib   # Windows Sockets
    user32.lib   # Windows API
    kernel32.lib # Windows Kernel API
    advapi32.lib # Advanced Windows API
    shell32.lib  # Windows Shell API
    ole32.lib    # Object Linking and Embedding API
    gdi32.lib    # Graphics Device Interface API
)

# Устанавливаем стандарт языка C++
target_compile_features(file_osc_module PRIVATE cxx_std_11)

# Устанавливаем свойства для совместимости с Python
set_target_properties(file_osc_module PROPERTIES
    PREFIX ""                      # Убираем префикс "lib"
    OUTPUT_NAME "Aegis_osc"        # Имя модуля
    SUFFIX ".pyd"                  # Расширение для Windows
    LANGUAGE CXX                   # Указываем язык компиляции как C++
)

# Печать сообщения об успешной настройке
message(STATUS "CMake configuration complete.")

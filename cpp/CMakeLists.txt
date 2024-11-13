# Устанавливаем минимальную версию CMake и имя проекта
cmake_minimum_required(VERSION 3.11)
project(Aegis_osc_project)

# Добавляем флаг для увеличения допустимого количества секций в объектном файле
add_compile_options(/bigobj)

# Устанавливаем политику CMP0148 для использования FindPython
if(POLICY CMP0148)
    cmake_policy(SET CMP0148 NEW)
endif()

# Указываем путь к pybind11 вручную
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../.venv/Lib/site-packages/pybind11/share/cmake/pybind11")

# Устанавливаем переменную для поиска Python через FindPython
set(PYBIND11_FINDPYTHON ON)

# Подключаем директории для include-файлов
include_directories(${CMAKE_SOURCE_DIR}/gl)
include_directories(${CMAKE_SOURCE_DIR}/cpp)
include_directories(${CMAKE_SOURCE_DIR}/libraries)

# Включаем поддержку MFC
add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 1)

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

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

include_directories(${CMAKE_SOURCE_DIR}/gl)
include_directories(${CMAKE_SOURCE_DIR}/сpp)
include_directories(${CMAKE_SOURCE_DIR}/libraries)

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 1)

# Используем pybind11
find_package(pybind11 REQUIRED)

# Добавляем файл с исходным кодом
add_library(file_osc_module MODULE cpp/file_osc.cpp)

# Настраиваем цель для сборки модуля
target_link_libraries(file_osc_module PRIVATE pybind11::module)
#target_link_libraries(file_osc_module PUBLIC ${CMAKE_SOURCE_DIR}) 
# Устанавливаем стандарт языка C++
target_compile_features(file_osc_module PRIVATE cxx_std_11)

# Устанавливаем свойства для совместимости с Python
set_target_properties(file_osc_module PROPERTIES
    PREFIX ""                      # Убираем префикс "lib"
    OUTPUT_NAME "Aegis_osc"          # Имя модуля
    SUFFIX ".pyd"                  # Расширение для Windows
    LANGUAGE CXX                   # Указываем язык компиляции как C++
)

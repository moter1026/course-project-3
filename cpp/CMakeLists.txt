
cmake_minimum_required(VERSION 3.11)

project(Aegis_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd") 

# Специфичные настройки для компилятора Visual Studio
if(MSVC)
    # Включаем высокий уровень предупреждений
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/gl)
include_directories(${CMAKE_SOURCE_DIR}/сpp)
include_directories(${CMAKE_SOURCE_DIR}/libraries)


file(GLOB SOURCES "cpp/*.cpp")

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 1)

add_executable (Aegis_cpp_exe "prosto_file.cpp")
target_sources(Aegis_cpp_exe PUBLIC ${SOURCES})
target_link_libraries(Aegis_cpp_exe PUBLIC ${CMAKE_SOURCE_DIR}) 


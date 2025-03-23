# Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(ParallelProgramming)

# Установка стандарта C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавление исходного файла
add_executable(lab-1 lab-1/src/main.cpp)
add_executable(lab-2 lab-2/src/main.cpp)

configure_file(
    ${CMAKE_SOURCE_DIR}/lab-1/src/config.txt
    ${CMAKE_BINARY_DIR}/config.txt
    COPYONLY
)
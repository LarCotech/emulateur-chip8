cmake_minimum_required(VERSION 3.24)
project(emulateur_chip8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "/Users/carineraffin-peyloz/Desktop")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(lib)

add_executable(emulateur_chip8
        main.c
        ram.c
        ram.h
        proc.h
        proc.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
# Ajouter la bibliothèque provided
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/debug/mingw64/libprovided.a)
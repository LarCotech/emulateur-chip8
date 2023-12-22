cmake_minimum_required(VERSION 3.26)
project(emulateur_chip8 C)

set(CMAKE_C_STANDARD 11)

add_executable(emulateur_chip8
        main.c
        ram.c
        ram.h
        prop.h
        prop.c)

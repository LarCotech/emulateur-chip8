cmake_minimum_required(VERSION 3.24)
project(emulateur_chip8 C)

set(CMAKE_C_STANDARD 11)

add_executable(emulateur_chip8
        main.c
        ram.c
        ram.h
        proc.h
        proc.c)

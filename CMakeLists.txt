cmake_minimum_required(VERSION 4.1)
project(Edu_sys)

set(CMAKE_CXX_STANDARD 20)

add_executable(Edu_sys main.cpp
        headers/funcs.h
        src/funcs.cpp
        headers/models.h)

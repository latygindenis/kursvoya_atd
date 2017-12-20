cmake_minimum_required(VERSION 3.9)
project(kursvoya_atd)

set(CMAKE_CXX_STANDARD 11)

add_executable(kursvoya_atd main.cpp ATD.cpp ATD.h Keynote.h)
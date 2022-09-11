cmake_minimum_required(VERSION 3.16)

project(acudp VERSION 1.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
add_library(
    acudp
    STATIC 
    src/acudp.c
    src/format.c
)

add_subdirectory(examples)
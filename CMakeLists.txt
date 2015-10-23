cmake_minimum_required(VERSION 3.3)
project(cargador)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c)
add_executable(cargador ${SOURCE_FILES})
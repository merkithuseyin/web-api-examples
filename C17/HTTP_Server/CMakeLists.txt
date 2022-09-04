cmake_minimum_required(VERSION 3.23)
project(HTTP_Server C)

set(CMAKE_C_STANDARD 17)

add_executable(HTTP_Server main.c httpserver.c httpserver.h)

cmake_minimum_required(VERSION 3.19)

project(vectorlib)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC src/vector.cpp include/vector.h)


cmake_minimum_required(VERSION 3.19)
project(vector_exec)

include_directories(vector/include)
include_directories(tests)

add_subdirectory(vector)
add_subdirectory(tests)
add_subdirectory(external/googletest)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} vectorlib)
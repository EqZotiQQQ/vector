cmake_minimum_required(VERSION 3.19)

project(vector_exec)

set(CMAKE_CXX_STANDARD 20)

include_directories(vector/include)
include_directories(tests)
include_directories(benchmark)

add_subdirectory(vector)
add_subdirectory(tests)
add_subdirectory(benchmark)

add_subdirectory(external/googletest)
add_subdirectory(external/benchmark)

add_compile_options(-Wall -Wextra -pedantic -Werror)

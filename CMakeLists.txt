cmake_minimum_required(VERSION 3.19)
project(vector_exec)

include_directories(vector/include)
include_directories(tests)

add_subdirectory(vector)
add_subdirectory(tests)
add_subdirectory(external/googletest)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -pedantic -Werror)

EXECUTE_PROCESS( COMMAND ${CMAKE_CXX_COMPILER} --version OUTPUT_VARIABLE clang_full_version_string )
string (REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" CLANG_VERSION_STRING ${clang_full_version_string})

message("Clang version is: ${CLANG_VERSION_STRING}")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} vectorlib)
project(vector_benchmark)

set(CMAKE_CXX_STANDARD 20)

include_directories(../vector/include)

add_executable(${PROJECT_NAME} benchmark.cpp)

target_link_libraries(${PROJECT_NAME} vectorlib)
target_link_libraries(${PROJECT_NAME} benchmark::benchmark)
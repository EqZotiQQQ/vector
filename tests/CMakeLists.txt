project(vector_tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} tests.cpp mock_objects.h)

target_link_libraries(${PROJECT_NAME} vectorlib)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
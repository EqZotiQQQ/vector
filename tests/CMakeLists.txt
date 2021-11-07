project(vector_tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} tests.cpp ADefault.h A0_mock.h A3_mock.h A5_mock.h)

target_link_libraries(${PROJECT_NAME} vectorlib)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
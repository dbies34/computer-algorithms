
project(HW5)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_BUILD_TYPE Debug)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# create unit test executable
add_executable(hw5_tests
               graph.cpp
               hw5_tests.cpp
               adjacency_list.cpp
               adjacency_matrix.cpp)
target_link_libraries(hw5_tests ${GTEST_LIBRARIES} pthread)


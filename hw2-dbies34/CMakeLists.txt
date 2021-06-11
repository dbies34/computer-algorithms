
project(CPSC_450_HW2)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_BUILD_TYPE Debug)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# create unit test executable
add_executable(hw2_tests
               hw2_tests.cpp
               adjacency_list.cpp
	       adjacency_matrix.cpp)
target_link_libraries(hw2_tests ${GTEST_LIBRARIES} pthread)

# create example graph tests
add_executable(hw2_examples
               hw2_examples.cpp
               adjacency_list.cpp
	       adjacency_matrix.cpp)
target_link_libraries(hw2_examples ${GTEST_LIBRARIES} pthread)


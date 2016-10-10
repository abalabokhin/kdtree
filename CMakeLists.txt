# Specify the minimum CMAKE version required

# cmake_minimum_required(VERSION 3.1)
# set (CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Your project's name
project(kdtree)

enable_testing()

# Make your header file accessible to the compiler
include_directories(./include)

# Instruct CMake to inspect the following subfolders
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

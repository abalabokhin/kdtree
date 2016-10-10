cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set (SRC kdtree.cpp)
set(INCLUDE ../include/kdtree.hpp
    ../include/kdpoint.hpp
    ../include/kdtreenode.hpp
    ../include/kdtreeleafnode.hpp
    ../include/kdtreeintermediatenode.hpp
    ../include/kdpointstorage.hpp
    )

# Define our fizzbuzz library. Our library does not have
# any dependencies
add_library(kdtreelib ${SRC} ${INCLUDE})

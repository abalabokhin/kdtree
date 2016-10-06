set (SRC kdtree.cpp)
set(INCLUDE ../include/kdtree.hpp ../include/kdpoint.hpp ../include/kdtreenode.hpp)

# Define our fizzbuzz library. Our library does not have
# any dependencies
add_library(kdtreelib ${SRC} ${INCLUDE})

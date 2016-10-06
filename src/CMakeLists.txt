set (SRC kdtree.cpp kpoint.cpp)
set(INCLUDE ../include/kdtree.hpp ../include/kpoint.hpp)

# Define our fizzbuzz library. Our library does not have
# any dependencies
add_library(kdtreelib ${SRC} ${INCLUDE})

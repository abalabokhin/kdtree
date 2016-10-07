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

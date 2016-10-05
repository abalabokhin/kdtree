set(SRC main.cpp)
set(INCLUDE ../include/kdtree.hpp)

# Define an executable and the libraries in depends on
add_executable(kdtree_app ${SRC} ${INCLUDE})
target_link_libraries(kdtree_app kdtreelib)

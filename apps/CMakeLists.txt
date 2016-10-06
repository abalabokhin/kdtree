set(SRC main.cpp)

# Define an executable and the libraries in depends on
add_executable(kdtree_app ${SRC})
target_link_libraries(kdtree_app kdtreelib)

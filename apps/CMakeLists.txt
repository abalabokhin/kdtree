find_package( Boost REQUIRED COMPONENTS serialization )
include_directories( ${Boost_INCLUDE_DIRS} )

set(SRC main.cpp)

# Define an executable and the libraries in depends on
add_executable(kdtree_app ${SRC})
target_link_libraries(kdtree_app kdtreelib)

# Alternatively you could use ${Boost_LIBRARIES} here.
target_link_libraries( kdtree_app ${Boost_SERIALIZATION_LIBRARY})

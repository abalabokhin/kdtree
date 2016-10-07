find_package( Boost REQUIRED COMPONENTS serialization )
include_directories( ${Boost_INCLUDE_DIRS} )

# Define an executable and the libraries in depends on
add_executable(build_kdtree build_kdtree.cpp)
target_link_libraries(build_kdtree kdtreelib ${Boost_SERIALIZATION_LIBRARY})

# Define an executable and the libraries in depends on
add_executable(query_kdtree query_kdtree.cpp)
target_link_libraries(query_kdtree kdtreelib ${Boost_SERIALIZATION_LIBRARY})

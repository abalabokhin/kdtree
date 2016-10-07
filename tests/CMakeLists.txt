# Necessary for the custom command tests
include(CTest)

# Assign all the libraries necessary to compile the test
# to the variable TEST_LIBRARIES
#set(TEST_LIBRARIES
#  kdtreelib
#  )

find_package( Boost REQUIRED COMPONENTS serialization unit_test_framework)
include_directories( ${Boost_INCLUDE_DIRS} )

# Define an executable and the libraries it depends on
# This executable is not built by default, in order to get
# The tests built and run use the custom target tests
add_executable(t_kdtree test_kdtree.cpp test_kdpoint.cpp)

add_definitions( -DBOOST_TEST_DYN_LINK )

target_link_libraries(t_kdtree
    kdtreelib
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

# Define a test that uses the executable we just defined
add_test(NAME test_kdtree COMMAND t_kdtree)

# Define a custom target that will force the build of the
# tests and will actually run the tests
#add_custom_target(tests COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS t_kdtree)

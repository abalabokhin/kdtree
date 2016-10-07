//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( my_test1 )
{
    BOOST_CHECK( 1 == 1);
}

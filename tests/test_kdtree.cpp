#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

TEST_CASE( "KDTree class tests" ) {
    KDPoint<int> one({1, 2});
    KDPoint<int> two({1, 2});

    REQUIRE(0 == one.squareDistanceTo(two));
}

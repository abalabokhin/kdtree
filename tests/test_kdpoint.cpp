#include "catch.hpp"
#include "kdpoint.hpp"

TEST_CASE( "KDPoint class tests" ) {
    KDPoint<int> one({1, 2});
    KDPoint<int> two({1, 2});

    REQUIRE(0 == one.squareDistanceTo(two));
}

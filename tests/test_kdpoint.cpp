#include "kdpoint.hpp"

#define BOOST_TEST_MODULE KDTreeTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( KDPointTest_wrongIndex )
{
    KDPoint<float> p;
    BOOST_CHECK_EXCEPTION(p.at(2), std::out_of_range, [](std::out_of_range const &){return true;});
}

BOOST_AUTO_TEST_CASE( KDPointTest_distanceBetweenDifferenSizePoints )
{
    KDPoint<float> p1({1, 2});
    KDPoint<float> p2({1, 2, 3});
    BOOST_CHECK_EXCEPTION(p1.squareDistanceToPoint(p2),
                          std::length_error, [](std::length_error const &){return true;});
}

BOOST_AUTO_TEST_CASE( KDPointTest_correctDistance )
{
    KDPoint<float> p1({-4, 2, 3});
    KDPoint<float> p2({1, -1, 5});
    BOOST_CHECK(p1.squareDistanceToPoint(p2) == 38);
}

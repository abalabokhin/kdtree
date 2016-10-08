#include "kdpointstorage.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( KDPointTest_differentPointsDimentions )
{
    BOOST_CHECK_EXCEPTION(
                KDPointStorage<float>({KDPoint<float>({1, 2}), KDPoint<float>({1, 2, 3})}, 2),
                std::domain_error, [](std::domain_error const &){return true;});
}

BOOST_AUTO_TEST_CASE( KDPointTest_findPivotAndPartitionAndClosestPointSearch )
{
    KDPointStorage<float> storage({KDPoint<float>({1, -1}),
                           KDPoint<float>({5, 3}),
                           KDPoint<float>({6, -4}),
                           KDPoint<float>({5, 5}),
                           KDPoint<float>({10, -6}),
                           KDPoint<float>({-3, 2})
                           }, 2);

    BOOST_CHECK_EQUAL(storage.findPivot(0, storage.size(), 0), 5);
    BOOST_CHECK_EQUAL(storage.partition(0, storage.size(), 0, 5), 2);

    /// points indecis: (0, 5), (1, 2, 3, 4)
    BOOST_CHECK_EQUAL(storage.findPivot(2, storage.size(), 1), 3);
    BOOST_CHECK_EQUAL(storage.partition(2, storage.size(), 1, 3), 4);

    /// points indecis: (0, 5), (2, 4), (3, 1)

    /// search among 0th and 5th poitns
    size_t i = 100;
    float minSDistance = 25;
    storage.findClosestPoint(KDPoint<float>({-2, 0}), minSDistance, i, 0, 2);
    BOOST_CHECK_EQUAL(i, 5);
    BOOST_CHECK(minSDistance < 5.001 );
}

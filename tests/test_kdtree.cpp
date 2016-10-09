
#include <kdtree.hpp>

#include <boost/test/unit_test.hpp>

#include <random>

KDPoint<float> generateKDRandomPoint(size_t K,
                                     std::uniform_real_distribution<> & dist,
                                     std::mt19937 & e2) {
    std::vector<float> coords(K);
    for (int coordI = 0; coordI < K; ++coordI) {
        coords[coordI] = dist(e2);
    }
    return KDPoint<float>(coords);
}

size_t findClosestPoint(
                    std::vector<KDPoint<float>> points,
                    KDPoint<float> const & p)
{

    size_t bestI = std::numeric_limits<size_t>::max();
    float minD = std::numeric_limits<float>::max();

    for (int i = 0; i < points.size(); ++i) {
        float dist = points[i].squareDistanceToPoint(p);
        if (dist < minD) {
            minD = dist;
            bestI = i;
        }
    }
    return bestI;
}

BOOST_AUTO_TEST_CASE( KDTreeTest_randomGeneratedTreesTest )
{
    /// test for random 100 points for dims for dims 1 to 5
    /// than test 1000 point to search

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(-1000, 1000);

    for (int dims = 1; dims < 5; ++dims) {
        std::vector<KDPoint<float>> points;
        for (int i = 0; i < 100; ++i) {
            points.push_back(generateKDRandomPoint(dims, dist, e2));
        }

        for (int pointsInFinalNode = 1; pointsInFinalNode < 5; ++pointsInFinalNode) {
            KDTree<float> tree(new KDPointStorage<float>(points, dims), pointsInFinalNode);

            for (int j = 0; j < 1000; ++j) {
                auto p = generateKDRandomPoint(dims, dist, e2);
                size_t bestPointI1 = 1000;
                auto closestPoint = tree.findClosestPoint(p, bestPointI1);
                size_t bestPointI2 = findClosestPoint(points, p);

                BOOST_CHECK_EQUAL(bestPointI1, bestPointI2);
                BOOST_CHECK(closestPoint == points[bestPointI2]);
            }
        }
    }
}

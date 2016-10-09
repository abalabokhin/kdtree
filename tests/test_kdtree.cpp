
#include <kdtree.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <random>

BOOST_CLASS_EXPORT(KDTreeIntermediateNode<float>)
BOOST_CLASS_EXPORT(KDTreeIntermediateNode<double>)

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

BOOST_AUTO_TEST_CASE( KDPointStorageTest_invalidTree )
{
    KDTree<float> tree;
    size_t i;
    BOOST_CHECK_EXCEPTION(
                tree.findClosestPoint(KDPoint<float>({1, 2}), i),
                std::domain_error, [](std::domain_error const &){return true;});
}

BOOST_AUTO_TEST_CASE( KDTreeTest_randomGeneratedTreesTest )
{
    /// test for random 100 points for dims for dims 1 to 5
    /// than test 1000 point to search
    /// it can crash somewhere, if two points are very close to each other or the same,
    /// but it is a good test that shown the correctness of the tree.

    /// the tree is also saved and restored
    /// it is better to have dedicated test and the way to compae trees
    /// but we don't have the method to compare two trees, so it is a good way to check
    /// that the restored tree is the correct one

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

            std::stringstream ss;
            {
                boost::archive::text_oarchive oa{ss};
                oa << tree;
            }

            KDTree<float> restoredTree;
            {
                boost::archive::text_iarchive ia{ss};
                ia >> restoredTree;
            }

            for (int j = 0; j < 1000; ++j) {
                auto p = generateKDRandomPoint(dims, dist, e2);
                size_t bestPointI1 = 1000;
                auto closestPoint = restoredTree.findClosestPoint(p, bestPointI1);
                size_t bestPointI2 = findClosestPoint(points, p);

                BOOST_CHECK_EQUAL(bestPointI1, bestPointI2);
                BOOST_CHECK(closestPoint == points[bestPointI2]);
            }
        }
    }
}

BOOST_AUTO_TEST_CASE( KDTreeTest_theSamePointsInTree )
{
    /// The tree should be correctly created even if it is created from the same points
    /// that is a very bad case to use tree
    std::vector<KDPoint<float>> points;
    KDPoint<float> point({1, 1});
    /// have 10 the same points
    for (int i = 0; i < 10; ++i) {
        points.push_back(point);
    }

    KDTree<float> tree(new KDPointStorage<float>(points, 2), 2);
    BOOST_CHECK_EQUAL(tree.getDepth(), 1);
}

BOOST_AUTO_TEST_CASE( KDTreeTest_differentDepths )
{
    std::vector<KDPoint<float>> points;
    KDPoint<float> point({1, 1});
    /// add 6 far points
    points.push_back(KDPoint<float>({1, 2}));
    points.push_back(KDPoint<float>({2, 3}));
    points.push_back(KDPoint<float>({3, 4}));
    points.push_back(KDPoint<float>({4, 3}));
    points.push_back(KDPoint<float>({3, 2}));
    points.push_back(KDPoint<float>({2, 1}));

    /// the depth should be around log_2(nPoints / maxPointsNumberInLeafNode),
    /// but depends on how well we split points
    /// it is calculated manually for the given points
    {
        KDTree<float> tree(new KDPointStorage<float>(points, 2), 1);
        BOOST_CHECK_EQUAL(tree.getDepth(), 4);
    }

    {
        KDTree<float> tree(new KDPointStorage<float>(points, 2), 2);
        BOOST_CHECK_EQUAL(tree.getDepth(), 3);
    }

    {
        KDTree<float> tree(new KDPointStorage<float>(points, 2), 3);
        BOOST_CHECK_EQUAL(tree.getDepth(), 2);
    }

    {
        KDTree<float> tree(new KDPointStorage<float>(points, 2), 6);
        BOOST_CHECK_EQUAL(tree.getDepth(), 1);
    }
}

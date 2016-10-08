#include <kdtreeintermediatenode.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( KDTreeIntermediateNode_getCloserSubNode )
{
    KDTreeIntermediateNode<float> node(1, 0.5);
    auto leftChild = new KDTreeIntermediateNode<float>();
    auto rightChild = new KDTreeIntermediateNode<float>();
    node.setLeftSubNode(leftChild);
    node.setRightSubNode(rightChild);

    BOOST_CHECK(rightChild == node.getCloserSubNode(KDPoint<float>({20.0, 1.1})));
    BOOST_CHECK(leftChild == node.getCloserSubNode(KDPoint<float>({-20.0, 0.49})));
    BOOST_CHECK(rightChild == node.getCloserSubNode(KDPoint<float>({0.0, 0.5})));
}

BOOST_AUTO_TEST_CASE( KDTreeIntermediateNode_addNodesToSearch )
{
    KDTreeIntermediateNode<float> node(0, -0.5);
    auto leftChild = new KDTreeIntermediateNode<float>();
    auto rightChild = new KDTreeIntermediateNode<float>();
    node.setLeftSubNode(leftChild);
    node.setRightSubNode(rightChild);

    std::vector<IKDTreeNode *> nodesToSearch;
    node.addNodesToSearch(nodesToSearch, KDPoint<float>({1, 38}), 4);
    BOOST_CHECK_EQUAL(nodesToSearch.size(), 2);
    BOOST_CHECK(nodesToSearch[0] == leftChild);
    BOOST_CHECK(nodesToSearch[1] == rightChild);

    node.addNodesToSearch(nodesToSearch, KDPoint<float>({-1.6, 200}), 1);
    BOOST_CHECK_EQUAL(nodesToSearch.size(), 3);
    BOOST_CHECK(nodesToSearch[2] == leftChild);

    node.addNodesToSearch(nodesToSearch, KDPoint<float>({1, -100.1}), 1);
    BOOST_CHECK_EQUAL(nodesToSearch.size(), 4);
    BOOST_CHECK(nodesToSearch[3] == rightChild);
}

#include <kdpoint.hpp>
#include <kdtree.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <sstream>
#include <iostream>

int main(int argc, char** argv) {
//    boost::serialization::void_cast_register<KDTreeIntermediateNode<double>, IKDTreeNode>();
//    boost::serialization::void_cast_register<KDTreeIntermediateNode<float>, IKDTreeNode>();
//    boost::serialization::void_cast_register<KDTreeLeafNode<double>, IKDTreeNode>();
//    boost::serialization::void_cast_register<KDTreeLeafNode<float>, IKDTreeNode>();


    std::cout << "Hellow World!" << std::endl;

    KDPoint<float> one({0, 0});
    KDPoint<float> two({2, 2});
    KDPoint<float> three({-1, -1});

    std::cout << one.squareDistanceToPoint(two);

    std::vector<KDPoint<float>> points{one, two, three};

//    KDTree<float> tree(points, 2);

//    size_t i = tree.findClosestPoint(KDPoint<float>({-0.6, -0.5}));

    /// test something
    std::stringstream ss;
    boost::archive::text_oarchive oa{ss};
    KDTree<float> tree(points, 2);
    oa << tree;

    KDTree<float> tree1(points, 2);
    boost::archive::text_iarchive ia{ss};
    ia >> tree1;

    ///
    int fgh = 78;
    ++fgh;
}

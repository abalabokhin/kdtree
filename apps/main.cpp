#include <kdpoint.hpp>
#include <kdtree.hpp>

#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Hellow World!" << std::endl;

    KDPoint<float> one({0, 0});
    KDPoint<float> two({2, 2});
    KDPoint<float> three({-1, -1});

    std::cout << one.squareDistanceToPoint(two);

    std::vector<KDPoint<float>> points{one, two, three};

    KDTree<float> tree(points, 2);

    size_t i = tree.findClosestPoint(KDPoint<float>({-0.6, -0.5}));

    int fgh = 78;
    ++fgh;
}


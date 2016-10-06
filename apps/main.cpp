#include <kdpoint.hpp>

#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Hellow World!" << std::endl;

    KDPoint<float> one({1, 2.0});
    KDPoint<float> two({1.2, 45});

    std::cout << one.squareDistanceToPoint(two);


}


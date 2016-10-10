#include <kdpoint.hpp>
#include <kdtree.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

BOOST_CLASS_EXPORT(KDTreeIntermediateNode<float>)
BOOST_CLASS_EXPORT(KDTreeIntermediateNode<double>)

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "This software exactly accepts two arguments in order: \n"
                     "1) input CSV file with points to build the kdtree for them\n"
                     "2) ouput file to save the built tree" << std::endl;
        return 1;
    }

    std::string csvFilename(argv[1]);
    std::string treeFilename(argv[2]);

    std::ifstream infile(csvFilename);
    if (!infile) {
        std::cout << csvFilename + " file is not found" << std::endl;
        return 1;
    }

    std::vector<KDPoint<double>> points;
    std::string line;
    while (std::getline(infile, line))
    {
        std::vector<double> coords;
        std::istringstream iss(line);
        std::string item;
        while (std::getline(iss, item, ',')) {
            coords.push_back(std::stod(item));
        }
        KDPoint<double> point(coords);
        points.push_back(point);
    }

    if (points.empty()) {
        std::cout << "there are no valiue points in the file: " << csvFilename << std::endl;
        return 1;
    }

    auto storage = new KDPointStorage<double>(points, points[0].size());
    KDTree<double> tree(storage, 2);

    std::ofstream outfile(treeFilename);
    boost::archive::text_oarchive oa{outfile};
    oa << tree;

//    KDPoint<float> one({0, 0});
//    KDPoint<float> two({2, 2});
//    KDPoint<float> three({-1, -1});

//    std::cout << one.squareDistanceToPoint(two);

//    std::vector<KDPoint<float>> points{one, two, three};

//    KDTree<float> tree(points, 2);

//    size_t i = tree.findClosestPoint(KDPoint<float>({-0.6, -0.5}));

//    /// test something
//    std::stringstream ss;
//    boost::archive::text_oarchive oa{ss};
//    auto storage = new KDPointStorage<float>(points, 2);
//    KDTree<float> tree(storage);
//    size_t i = 0;
//    tree.findClosestPoint(KDPoint<float>({1.2, 3.4}), i);
//    oa << tree;


//    KDTree<float> tree1;
//    boost::archive::text_iarchive ia{ss};
//    ia >> tree1;

//    ///
//    int fgh = 78;
//    ++fgh;

    return 0;
}

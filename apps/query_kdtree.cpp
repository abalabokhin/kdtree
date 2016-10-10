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
    if (argc != 4) {
        std::cout << "This software accepts three arguments exactly. They are: \n"
                     "1) input file having valid built k-d tree\n"
                     "2) input CSV file with points to search in the tree\n"
                     "3) ouput file to save the indices and the distances of the closest "
                     "points from the tree" << std::endl;
        return 1;
    }

    std::string csvFilename(argv[2]);
    std::string treeFilename(argv[1]);
    std::string outputFilename(argv[3]);

    /// read tree from file
    std::ifstream treeFile(treeFilename);
    if (!treeFile) {
        std::cout << treeFilename + " file is not found" << std::endl;
        return 1;
    }
    KDTree<double> tree;
    boost::archive::text_iarchive ia{treeFile};
    ia >> tree;

    /// read points from file
    std::ifstream infile(csvFilename);
    if (!infile) {
        std::cout << csvFilename + " file is not found" << std::endl;
        return 1;
    }

    std::ofstream outfile(outputFilename);

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

        size_t i = 0;
        auto & closestPoint = tree.findClosestPoint(point, i);

        outfile << i << ", " << sqrt(closestPoint.squareDistanceToPoint(point)) << std::endl;
    }
    return 0;
}

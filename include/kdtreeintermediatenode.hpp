#pragma once

#include <kdpoint.hpp>
#include <kdtreenode.hpp>
#include <boost/scoped_ptr.hpp>

/// Intermediate node of kd-tree that stores splitting plane info and left and right subtrees
template <typename T>
class KDTreeIntermediateNode : public IKDTreeNode
{
public:
    /// Empty c-tor for serialization
    KDTreeIntermediateNode() {}

    KDTreeIntermediateNode(size_t aPlaneCoordinateI, T aPlaneCoordinate)
        : planeCoordinateI(aPlaneCoordinateI), planeCoordinate(aPlaneCoordinate)
    {}

    /// find the node in subnodes that has the provided point
    IKDTreeNode * getCloserSubNode(KDPoint<T> const & p) {
        if (p.at(planeCoordinateI) < planeCoordinate) {
            return leftSubNode.get();
        } else {
            return rightSubNode.get();
        }
    }

    /// check the distance from the point to the boiundary of the subnodes
    /// and add either both nodes or only one to search further
    void addNodesToSearch(std::vector<IKDTreeNode *> & nodesToSearch,
                          KDPoint<T> const & p,
                          T minSquareDistance)
    {
        T distance = (p.at(planeCoordinateI) - planeCoordinate);
        if (distance * distance < minSquareDistance + std::numeric_limits<T>::epsilon()) {
            nodesToSearch.push_back(leftSubNode.get());
            nodesToSearch.push_back(rightSubNode.get());
        } else {
            nodesToSearch.push_back(getCloserSubNode(p));
        }
    }

    void setLeftSubNode(IKDTreeNode * node) {
        leftSubNode.reset(node);
    }

    void setRightSubNode(IKDTreeNode * node) {
        rightSubNode.reset(node);
    }

private:
    /// Boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        boost::serialization::void_cast_register<KDTreeIntermediateNode<T>, IKDTreeNode>();
        boost::serialization::base_object<IKDTreeNode>(*this);
        ar & planeCoordinateI & planeCoordinate & leftSubNode & rightSubNode;
    }

    size_t planeCoordinateI = 0;
    T planeCoordinate = 0;
    boost::scoped_ptr<IKDTreeNode> leftSubNode;
    boost::scoped_ptr<IKDTreeNode> rightSubNode;
};

/// If you need serialization for KDTreeIntermediateNode<T> make sure that you registered the class
/// before using serialization in cpp. E.g:
/// BOOST_CLASS_EXPORT(KDTreeIntermediateNode<float>)
/// BOOST_CLASS_EXPORT(KDTreeIntermediateNode<double>)




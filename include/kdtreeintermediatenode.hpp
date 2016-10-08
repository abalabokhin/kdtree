#pragma once

#include<kdtreenode.hpp>

/// if node an intermediate one
template <typename T>
class KDTreeIntermediateNode : public IKDTreeNode
{
public:
    KDTreeIntermediateNode() {}

    KDTreeIntermediateNode(size_t aPlaneCoordinateI, T aPlaneCoordinate)
        : planeCoordinateI(aPlaneCoordinateI), planeCoordinate(aPlaneCoordinate)
    {}

    IKDTreeNode * getCloserSubNode(KDPoint<T> const & p) {
        if (p.at(planeCoordinateI) < planeCoordinate) {
            return leftSubNode.get();
        } else {
            return rightSubNode.get();
        }
    }

    void addNodesToSearch(std::vector<IKDTreeNode *> & nodesToSearch,
                          KDPoint<T> const & p,
                          T minSquareDistance)
    {
        T distance = (p.at(planeCoordinateI) - planeCoordinate);
        if (distance * distance < minSquareDistance) {
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
    /// It is necessary to use boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        boost::serialization::void_cast_register<KDTreeIntermediateNode<T>, IKDTreeNode>();
        boost::serialization::base_object<IKDTreeNode>(*this);
        ar & planeCoordinateI & planeCoordinate & leftSubNode & rightSubNode;
    }

    size_t planeCoordinateI = 0;
    T planeCoordinate = 0;
    std::unique_ptr<IKDTreeNode> leftSubNode;
    std::unique_ptr<IKDTreeNode> rightSubNode;
};

/// If you need serialization for KDTreeIntermediateNode<T> make sure that you registered the class
/// before using serialization in cpp once. E.g:
/// BOOST_CLASS_EXPORT(KDTreeIntermediateNode<float>)
/// BOOST_CLASS_EXPORT(KDTreeIntermediateNode<double>)




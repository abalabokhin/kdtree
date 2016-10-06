#pragma once

#include <memory>

/// Interface for KDTReeNode
template <typename T>
class IKDTreeNode
{
public:
    virtual ~IKDTreeNode() {}
};

/// if node is a leaf
template<typename T> class KDTreeLeafNode : public IKDTreeNode<T>
{
public:
    KDTreeLeafNode(size_t aLeftPointsIndecisI, size_t aRightPointsIndecisI)
        : leftPointsIndecisI(aLeftPointsIndecisI), rightPointsIndecisI(aRightPointsIndecisI)
    {}
private:
    size_t leftPointsIndecisI;
    size_t rightPointsIndecisI;
};

/// if node an intermediate one
template<typename T> class KDTreeIntermediateNode : public IKDTreeNode<T>
{
public:
    KDTreeIntermediateNode(size_t aPlaneCoordinateI, T aPlaneCoordinate)
        : planeCoordinateI(aPlaneCoordinateI), planeCoordinate(aPlaneCoordinate)
    {}

    void setLeftSubNode(IKDTreeNode<T> * node) {
        leftSubNode.reset(node);
    }

    void setRightSubNode(IKDTreeNode<T> * node) {
        rightSubNode.reset(node);
    }

private:
    size_t planeCoordinateI = 0;
    T planeCoordinate;
    std::unique_ptr<IKDTreeNode<T>> leftSubNode;
    std::unique_ptr<IKDTreeNode<T>> rightSubNode;
};

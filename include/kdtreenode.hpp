#pragma once

#include <kdpoint.hpp>

#include <memory>

/// Interface for KDTReeNode
class IKDTreeNode
{
public:
    virtual ~IKDTreeNode() {}
};

/// if node is a leaf
template <typename T>
class KDTreeLeafNode : public IKDTreeNode
{
public:
    KDTreeLeafNode(size_t aLeftPointsIndecisI, size_t aRightPointsIndecisI)
        : leftPointsIndecisI(aLeftPointsIndecisI), rightPointsIndecisI(aRightPointsIndecisI)
    {}

    void findClosestPoint(
            std::vector<KDPoint<T>> const & points,
            std::vector<size_t> const & indices,
            KDPoint<T> const & p,
            T & minSquareDistance,
            size_t & bestI
        ) const
    {
        for (size_t i = leftPointsIndecisI; i < rightPointsIndecisI; ++i) {
            auto squareDistanceCandidate = points.at(indices[i]).squareDistanceToPoint(p);
            if (squareDistanceCandidate < minSquareDistance) {
                minSquareDistance = squareDistanceCandidate;
                bestI = indices[i];
            }
        }
    }

private:
    size_t leftPointsIndecisI;
    size_t rightPointsIndecisI;
};

/// if node an intermediate one
template <typename T>
class KDTreeIntermediateNode : public IKDTreeNode
{
public:
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
    size_t planeCoordinateI = 0;
    T planeCoordinate;
    std::unique_ptr<IKDTreeNode> leftSubNode;
    std::unique_ptr<IKDTreeNode> rightSubNode;
};

#pragma once

#include <kdtreenode.hpp>
#include <kdpoint.hpp>

#include <boost/serialization/unique_ptr.hpp>

#include <cstddef>
#include <memory>
#include <algorithm>
#include <limits>

template <typename T>
class KDTree {
public:
    /// empty c-tor for serialization.
    KDTree() {}

    KDTree(KDPointStorage<T> * aStorage, size_t aMaxNumberOfPointsInLeafNode = 1)
        : maxNumberOfPointsInLeafNode(aMaxNumberOfPointsInLeafNode)
    {
        storage.reset(aStorage);
        root.reset(buildTree(0, storage->size(), 0));
    }

    size_t findClosestPoint(KDPoint<T> const & p) {
        size_t closestPointOriginalI = findAClosePoint(p, root.get());
        T minSquareDistance = storage.getPointByOriginalI(closestPointOriginalI).
                squareDistanceToPoint(p);

        std::vector<IKDTreeNode *> nodesToSearch;
        nodesToSearch.push_back(root.get());

        while(!nodesToSearch.empty()) {
            auto node = nodesToSearch.back();
            nodesToSearch.pop_back();
            if (KDTreeLeafNode * leaf = dynamic_cast<KDTreeLeafNode *>(node)) {
                storage->findClosestPoint(
                            p,
                            minSquareDistance,
                            closestPointOriginalI,
                            leaf->getLeftI(),
                            leaf->getRightI()
                            );
            } else {
                KDTreeIntermediateNode<T> * intermediateNode =
                        dynamic_cast<KDTreeIntermediateNode<T> *>(node);

                intermediateNode->addNodesToSearch(nodesToSearch, p, minSquareDistance);
            }
        }
        return closestPointOriginalI;
    }

private:
    /// return index of a close point in the original point list
    size_t findAClosePoint(KDPoint<T> const & p, IKDTreeNode * node) {
        if (KDTreeLeafNode * leaf = dynamic_cast<KDTreeLeafNode *>(node)) {
            size_t closestPointI = std::numeric_limits<size_t>::max();
            T minSquareDistance = std::numeric_limits<T>::max();

            storage->findClosestPoint(
                        p,
                        minSquareDistance,
                        closestPointI,
                        leaf->getLeftI(),
                        leaf->getRightI()
                        );

            return closestPointI;
        } else {
            KDTreeIntermediateNode<T> * intermediateNode =
                    dynamic_cast<KDTreeIntermediateNode<T> *>(node);

            return findAClosePoint(p, intermediateNode->getCloserSubNode(p));
        }
    }

    IKDTreeNode * buildTree(size_t leftPointsIndicesI, size_t rightPointsIndicesI, size_t levelI)
    {
        if (rightPointsIndicesI <= leftPointsIndicesI) {
            throw std::length_error("left index must always be bigger than the right one");
        }
        if (rightPointsIndicesI - leftPointsIndicesI <= maxNumberOfPointsInLeafNode) {
            /// create a leaf node here
            new KDTreeLeafNode(leftPointsIndicesI, rightPointsIndicesI);
        } else {
            /// create an intermediate node here
            /// find a coordinateI to build a plane
            size_t splitingPlaneCoordinateI = storage->findSplittingPanelCoordinateI(
                        leftPointsIndicesI,
                        rightPointsIndicesI,
                        levelI
                        );
            /// find a pivot, an create two subtrees
            auto pivot = storage->findPivot(
                        leftPointsIndicesI,
                        rightPointsIndicesI,
                        splitingPlaneCoordinateI
                        );

            auto middlePointsIndicesI = storage->partition(
                        leftPointsIndicesI,
                        rightPointsIndicesI,
                        splitingPlaneCoordinateI,
                        pivot);

            auto * node = new KDTreeIntermediateNode<T>(splitingPlaneCoordinateI, pivot);
            node->setLeftSubNode(buildTree(leftPointsIndicesI, middlePointsIndicesI, levelI + 1));
            node->setRightSubNode(buildTree(middlePointsIndicesI, rightPointsIndicesI, levelI + 1));
            return node;
        }
    }

    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & maxNumberOfPointsInLeafNode & storage & root;
    }

    size_t maxNumberOfPointsInLeafNode = 1;
    std::unique_ptr<KDPointStorage<T>> storage;
    std::unique_ptr<IKDTreeNode> root;
};

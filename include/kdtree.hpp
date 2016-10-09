#pragma once

#include <kdtreeleafnode.hpp>
#include <kdtreeintermediatenode.hpp>
#include <kdpointstorage.hpp>

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

    KDTree(KDPointStorage<T> * aStorage, size_t aMaxPointsNumberInLeafNode = 1)
        : maxPointsNumberInLeafNode(aMaxPointsNumberInLeafNode)
    {
        storage.reset(aStorage);
        root.reset(buildTree(0, storage->size(), 0));
    }

    size_t getDepth() { return depth; }

    KDPoint<T> const & findClosestPoint(KDPoint<T> const & p, size_t & closestPointOriginalI) {
        if (!root || !storage || storage->size() == 0) {
            throw std::domain_error("tree or points storage is invalid");
        }
        closestPointOriginalI = findAClosePoint(p, root.get());
        T minSquareDistance = storage->getPointByOriginalI(closestPointOriginalI).
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
        return storage->getPointByOriginalI(closestPointOriginalI);
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
        depth = std::max(depth, levelI + 1);
        if (rightPointsIndicesI <= leftPointsIndicesI) {
            throw std::length_error("left index must always be bigger than the right one");
        }
        if (rightPointsIndicesI - leftPointsIndicesI <= maxPointsNumberInLeafNode) {
            /// create a leaf node here
            return new KDTreeLeafNode(leftPointsIndicesI, rightPointsIndicesI);
        } else {
            /// create an intermediate node here
            /// find a coordinateI to build a splitting plane
            size_t splitingPlaneCoordinateI = storage->findSplittingPanelCoordinateI(
                        leftPointsIndicesI,
                        rightPointsIndicesI,
                        levelI
                        );

            /// find a pivot two create two subtrees
            auto pivot = storage->findPivot(
                        leftPointsIndicesI,
                        rightPointsIndicesI,
                        splitingPlaneCoordinateI
                        );

            /// partition points around the pivot and get the index of
            auto middlePointsIndicesI = storage->partition(
                        leftPointsIndicesI,
                        rightPointsIndicesI,
                        splitingPlaneCoordinateI,
                        pivot);

            /// It should no be empty nodes. Thus, if a middle index not in between the left and
            /// the rigth one, leaf node is created.
            if (middlePointsIndicesI <= leftPointsIndicesI ||
                    middlePointsIndicesI >= rightPointsIndicesI) {
                return new KDTreeLeafNode(leftPointsIndicesI, rightPointsIndicesI);
            }

            auto * node = new KDTreeIntermediateNode<T>(splitingPlaneCoordinateI, pivot);
            node->setLeftSubNode(buildTree(leftPointsIndicesI, middlePointsIndicesI, levelI + 1));
            node->setRightSubNode(buildTree(middlePointsIndicesI, rightPointsIndicesI, levelI + 1));
            return node;
        }
    }

    /// It is necessary to use boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & maxPointsNumberInLeafNode & storage & root;
    }

    size_t depth = 0;
    size_t maxPointsNumberInLeafNode = 1;
    std::unique_ptr<KDPointStorage<T>> storage;
    std::unique_ptr<IKDTreeNode> root;
};

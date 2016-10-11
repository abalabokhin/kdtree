#pragma once

#include <kdtreeleafnode.hpp>
#include <kdtreeintermediatenode.hpp>
#include <kdpointstorage.hpp>

#include <boost/serialization/scoped_ptr.hpp>

#include <cstddef>
#include <algorithm>
#include <limits>

/// K-dimetional tree
template <typename T>
class KDTree {
public:
    /// empty c-tor for serialization.
    KDTree() {}

    /// It accepth the ownership of the storage, and delete it after using
    /// The tree is constracted here
    /// It doesn't know about K, this information is in storage.
    KDTree(KDPointStorage<T> * aStorage, size_t aMaxPointsNumberInLeafNode = 1)
        : maxPointsNumberInLeafNode(aMaxPointsNumberInLeafNode)
    {
        storage.reset(aStorage);
        root.reset(buildTree(0, storage->size(), 0));
    }

    size_t getDepth() const { return depth; }

    KDPoint<T> const & findClosestPoint(KDPoint<T> const & p, size_t & closestPointOriginalI) const {
        if (!root || !storage || storage->size() == 0) {
            throw std::domain_error("tree or points storage is invalid");
        }
        /// find the first candidate for the closest point
        closestPointOriginalI = findAClosePoint(p, root.get());
        T minSquareDistance = storage->getPointByOriginalI(closestPointOriginalI).
                squareDistanceToPoint(p);

        /// nodes to search in order to find the closest point
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
    /// It searches the closest point in the same node as the point to search is located.
    /// It is not optimal though, so this algorithm is only used to find a candidate to
    /// the closest point.
    /// returns index of a closest point in the original point list
    size_t findAClosePoint(KDPoint<T> const & p, IKDTreeNode * node) const {
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

    /// build one node of the tree
    IKDTreeNode * buildTree(size_t leftPointsI, size_t rightPointsI, size_t levelI)
    {
        depth = std::max(depth, levelI + 1);
        /// it is impossible situation, if everything is right
        if (rightPointsI <= leftPointsI) {
            throw std::length_error("left index must always be bigger than the right one");
        }
        /// time to create a leaf node, we have too few points to split
        if (rightPointsI - leftPointsI <= maxPointsNumberInLeafNode) {
            /// create a leaf node here
            return new KDTreeLeafNode(leftPointsI, rightPointsI);
        } else {
            /// create an intermediate node here
            /// find a coordinateI to build a splitting plane
            size_t splitingPlaneCoordinateI = storage->findSplittingPlaneCoordinateI(
                        leftPointsI,
                        rightPointsI,
                        levelI
                        );

            /// find a pivot to create two subtrees
            auto pivot = storage->findPivot(
                        leftPointsI,
                        rightPointsI,
                        splitingPlaneCoordinateI
                        );

            /// partition points around the pivot and get the index of the middle point
            auto middlePointsI = storage->partition(
                        leftPointsI,
                        rightPointsI,
                        splitingPlaneCoordinateI,
                        pivot);

            /// It should be no empty nodes. Thus, if a middle index not in between the left and
            /// the rigth one, leaf node is created.
            /// It can happen if we have identical points per the given coordinateI, for instance
            if (middlePointsI <= leftPointsI ||
                    middlePointsI >= rightPointsI) {
                return new KDTreeLeafNode(leftPointsI, rightPointsI);
            }

            /// build left and right subtrees
            auto * node = new KDTreeIntermediateNode<T>(splitingPlaneCoordinateI, pivot);
            node->setLeftSubNode(buildTree(leftPointsI, middlePointsI, levelI + 1));
            node->setRightSubNode(buildTree(middlePointsI, rightPointsI, levelI + 1));
            return node;
        }
    }

    /// Boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & maxPointsNumberInLeafNode & storage & root;
    }

    size_t depth = 0;
    size_t maxPointsNumberInLeafNode = 1;
    boost::scoped_ptr<KDPointStorage<T>> storage;
    boost::scoped_ptr<IKDTreeNode> root;
};

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

    KDTree(std::vector<KDPoint<T>> const & aPoints, size_t aK, size_t aMaxNumberOfPointsInLeafNode = 1)
    /// We can swap vectors here, but the copy is done only ones when the tree is created.
    /// Do it if it is a bottle neck in tree creation.
        : K(aK),
          maxNumberOfPointsInLeafNode(aMaxNumberOfPointsInLeafNode),
          points(aPoints),
          indices(points.size())
    {
        /// create list of indices, all the manipulation with points (sorting, partitioning, etc)
        /// is supposed to be performed with indices instead, because it is faster than manipuating
        /// with points directly
        for (int i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }

        root.reset(buildTree(0, indices.size(), 0));
    }

    size_t findClosestPoint(KDPoint<T> const & p) {
        size_t closestPointI = findAClosePoint(p, root.get());
        T minSquareDistance = points[closestPointI].squareDistanceToPoint(p);

        std::vector<IKDTreeNode *> nodesToSearch;
        nodesToSearch.push_back(root.get());

        while(!nodesToSearch.empty()) {
            auto node = nodesToSearch.back();
            nodesToSearch.pop_back();
            if (KDTreeLeafNode<T> * leaf = dynamic_cast<KDTreeLeafNode<T> *>(node)) {
                leaf->findClosestPoint(
                            points,
                            indices,
                            p,
                            minSquareDistance,
                            closestPointI
                            );
            } else {
                KDTreeIntermediateNode<T> * intermediateNode =
                        dynamic_cast<KDTreeIntermediateNode<T> *>(node);

                intermediateNode->addNodesToSearch(nodesToSearch, p, minSquareDistance);
            }
        }
        return closestPointI;
    }

private:
    /// return index of a close point in the original point list
    size_t findAClosePoint(KDPoint<T> const & p, IKDTreeNode * node) {
        if (KDTreeLeafNode<T> * leaf = dynamic_cast<KDTreeLeafNode<T> *>(node)) {
            size_t closestPointI = std::numeric_limits<size_t>::max();
            T minSquareDistance = std::numeric_limits<T>::max();

            leaf->findClosestPoint(points,
                                   indices,
                                   p,
                                   minSquareDistance,
                                   closestPointI
                                   );

            return closestPointI;
        } else {
            KDTreeIntermediateNode<T> * intermediateNode =
                    dynamic_cast<KDTreeIntermediateNode<T> *>(node);

            return findAClosePoint(p, intermediateNode->getCloserSubNode(p));
        }
    }

    size_t findSplittingPanelCoordinateI(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t levelI,
            std::vector<KDPoint<T>> const & points,
            std::vector<size_t> const & indices
            )
    {
        return levelI % K;
    }

    /// this functions should not do anyhting with the indecies outside of the (leftPointsIndecisI,
    /// rightPointsIndecisI) range
    T findPivot(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t coordinateI,
            std::vector<KDPoint<T>> const & points,
            std::vector<size_t> & indices
            )
    {
        /// find a median, now we just find left median, other algorithms can be used to have
        /// better ballanced tree, or find median faster.
        size_t middlePointsIndicesI = (rightPointsIndicesI + leftPointsIndicesI) / 2;

        std::nth_element(
                    indices.begin() + leftPointsIndicesI,
                    indices.begin() + middlePointsIndicesI,
                    indices.begin() + rightPointsIndicesI,
                    [&](size_t i, size_t j) {
                        auto elementI = points[i].at(coordinateI);
                        auto elementJ = points[j].at(coordinateI);
                        return elementI < elementJ;
                    }
        );

        return points.at(indices.at(middlePointsIndicesI)).at(coordinateI);
    }

    size_t partition(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t coordinateI,
            T pivot
            )
    {
        /// TODO: we can use iterators instead of indices
        auto middleI = std::partition(indices.begin() + leftPointsIndicesI,
                                      indices.begin() + rightPointsIndicesI,
                                      [&](size_t i)
        {
            auto elementI = points[i].at(coordinateI);
            bool temp = (elementI < pivot);
            return temp;
        }
        );

        return middleI - indices.begin();
    }

    IKDTreeNode * buildTree(size_t leftPointsIndicesI, size_t rightPointsIndicesI, size_t levelI)
    {
        if (rightPointsIndicesI <= leftPointsIndicesI) {
            throw std::length_error("left index must always be bigger than the right one");
        }
        if (rightPointsIndicesI - leftPointsIndicesI <= maxNumberOfPointsInLeafNode) {
            /// create a leaf node here
            new KDTreeLeafNode<T>(leftPointsIndicesI, rightPointsIndicesI);
        } else {
            /// create an intermediate node here
            /// find a coordinateI to build a plane
            size_t splitingPlaneCoordinateI = findSplittingPanelCoordinateI(
                        leftPointsIndicesI,
                        rightPointsIndicesI,
                        levelI,
                        points,
                        indices
                        );
            /// find a pivot, an create two subtrees
            auto pivot = findPivot(
                        leftPointsIndicesI,
                       rightPointsIndicesI,
                       splitingPlaneCoordinateI,
                       points,
                       indices);

            auto middlePointsIndicesI = partition(
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
        ar & K & maxNumberOfPointsInLeafNode & points & indices & root;
    }

    size_t K = 0;
    size_t maxNumberOfPointsInLeafNode = 0;
    /// TODO: probably have a dedicated class that hide the complexity of using indexis and points,
    /// it can store points and indexis together, and we can provide it with median finder )
    /// other altrnative that we can swap points cheap
    std::vector<KDPoint<T>> points;
    std::vector<size_t> indices;
    std::unique_ptr<IKDTreeNode> root;
};

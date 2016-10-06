#pragma once

#include <kdtreenode.hpp>
#include <kdpoint.hpp>
#include <cstddef>
#include <memory>
#include <algorithm>

template <typename T>
class KDtree {
public:
    KDtree(std::vector<KDPoint<T>> const & aPoints, size_t aK, size_t aMaxNumberOfPointsInLeafNode = 1)
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

        root = buildTree(0, indices.size(), 0);
    }

private:
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
        /// find a median
        /// sort indices in the range by having the corresponding points in acsending order
        std::sort(indices.data() + leftPointsIndicesI, indices.data() + rightPointsIndicesI,
            [&](size_t a, size_t b) {
                return points[indices[a]].at[coordinateI] <
                    points[indices[b]].at[coordinateI];
            }
        );

        if ((rightPointsIndicesI - leftPointsIndicesI) % 2 == 1) {
            return points[indices[(rightPointsIndicesI + leftPointsIndicesI) / 2]].at(coordinateI);
        } else {
            int biggerI = (rightPointsIndicesI + leftPointsIndicesI) / 2;
            auto m1 = points[indices[biggerI]].at(coordinateI);
            auto m2 = points[indices[biggerI - 1]].at(coordinateI);
            return (m1 + m2) / 2;
        }
    }

    size_t partition(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t coordinateI,
            T pivot
            )
    {
        /// TODO: parition indices here, now it is partitioned, cause the indices array
        /// is sorted, so just find the first element from right subtree and return it
        for (size_t i = leftPointsIndicesI; i < rightPointsIndicesI; ++i) {
            if (points[indices[i]].at(coordinateI) > pivot) {
                return i;
            }
        }
    }

    IKDTreeNode<T> * buildTree(size_t leftPointsIndicesI, size_t rightPointsIndicesI, size_t levelI)
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

            IKDTreeNode<T> node = new KDTreeIntermediateNode<T>(splitingPlaneCoordinateI, pivot);
            node->setLeftSubNode(buildTree(leftPointsIndicesI, middlePointsIndicesI, levelI + 1));
            node->setRightSubNode(buildTree(middlePointsIndicesI, rightPointsIndicesI, levelI + 1));
            return node;
        }
    }

    size_t K;
    size_t maxNumberOfPointsInLeafNode;
    /// TODO: probably have a dedicated class that hide the complexity of using indexis and points,
    /// it can store points and indexis together, and we can provide it with median finder )
    std::vector<KDPoint<T>> points;
    std::vector<size_t> indices;
    std::unique_ptr<IKDTreeNode<T>> root;
};

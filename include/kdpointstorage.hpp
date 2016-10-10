#pragma once

#include<kdpoint.hpp>

#include <exception>

/// In this class we inclapsulate the point storage. All the manipulation with points are performed
/// here, like partition, selecting pivot, selecting coordinate to split, etc.
/// findPivot and findSplittingPanelCoordinateI can be overrided to use other algorithms to
/// perform these operations.
template <typename T>
class KDPointStorage {
public:
    /// Empty c-tor for serialization
    KDPointStorage() {}

    virtual ~KDPointStorage() {}

    /// All the operations are performed with indices instead of the points directly,
    /// because it is faster and we need the index in the original array any way.
    /// Other implementations can be used instead.
    KDPointStorage(std::vector<KDPoint<T>> const & aPoints, size_t aK)
        : K(aK), points(aPoints), indices(points.size())
    {
        if (K == 0)
            throw std::domain_error("points dimension should be > 0");

        if (points.size() == 0)
            throw std::domain_error("point storage must have at least one point");

        for (int i = 0; i < points.size(); ++i) {
            if (points.at(i).size() != K)
                throw std::domain_error("point storage has different dimesion than some points");

            indices[i] = i;
        }
    }

    /// Can be overrided in derived classes to have other logic here.
    /// Now all the cordinates changed in order and depends on the depth of the current tree level.
    virtual size_t findSplittingPanelCoordinateI(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t levelI
            ) const
    {
        return levelI % K;
    }

    /// Can be overrided in derived classes to have other logic here.
    /// Find a pivot to split points on nodes in tree. In current implementation,
    /// this method can change the storage (and it is not const),
    /// but only within provided left and right indices. It was done for the performance.
    /// If part of the array is copied, this method can be const.
    ///
    /// The current implementation finds for the right median.
    virtual T findPivot(
            size_t leftPointsI,
            size_t rightPointsI,
            size_t coordinateI
            )
    {
        size_t middlePointsIndicesI = (rightPointsI + leftPointsI) / 2;

        std::nth_element(
                    indices.begin() + leftPointsI,
                    indices.begin() + middlePointsIndicesI,
                    indices.begin() + rightPointsI,
                    [&](size_t i, size_t j) {
                        auto elementI = points[i].at(coordinateI);
                        auto elementJ = points[j].at(coordinateI);
                        return elementI < elementJ;
                    }
        );

        return points.at(indices.at(middlePointsIndicesI)).at(coordinateI);
    }

    /// Partition points around pivot by the given coordinate.
    size_t partition(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t coordinateI,
            T pivot
            )
    {
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

    /// Search the closest points in the range
    void findClosestPoint(
            KDPoint<T> const & p,
            T & minSquareDistance,
            size_t & originalPointI,
            size_t leftI,
            size_t rightI
        ) const
    {
        for (size_t i = leftI; i < rightI; ++i) {
            auto squareDistanceCandidate = points.at(indices[i]).squareDistanceToPoint(p);
            if (squareDistanceCandidate < minSquareDistance) {
                minSquareDistance = squareDistanceCandidate;
                originalPointI = indices[i];
            }
        }
    }

    size_t size() const
    {
        return points.size();
    }

    /// return point reference by the index in the original points array order.
    KDPoint<T> const & getPointByOriginalI(size_t i) const {
        return points.at(i);
    }

protected:
    size_t K = 1;
    std::vector<KDPoint<T>> points;
    std::vector<size_t> indices;

private:
    /// Boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & K & points & indices;
    }
};

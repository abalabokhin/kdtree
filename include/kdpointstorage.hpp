#pragma once

/// In this class we abtract the point storage. All the manipulation with points are performed
/// here, like partition, selecting pivot, selecting coordinate to split, etc.
/// findPivot and findSplittingPanelCoordinateI can be overrided to use other algorithms to
/// perform this operations.
template <typename T>
class KDPointStorage {
public:
    KDPointStorage() {}
    virtual ~KDPointStorage() {}

    KDPointStorage(std::vector<KDPoint<T>> const & aPoints, size_t aK)
        : K(aK), points(aPoints), indices(points.size())
    {
        /// create list of indices, all the manipulation with points (sorting, partitioning, etc)
        /// is supposed to be performed with indices instead, because it is faster than manipuating
        /// with points directly

        /// TODO: check that all the points has the right dimention and K > 0
        for (int i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }
    }

    virtual size_t findSplittingPanelCoordinateI(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t levelI
            )
    {
        return levelI % K;
    }

    virtual T findPivot(
            size_t leftPointsIndicesI,
            size_t rightPointsIndicesI,
            size_t coordinateI
            )
    {
        /// find a median, now we just find left median, other algorithms can be used to have
        /// better ballanced tree, or find median faster. E.g median of some random
        /// points can be used

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

    KDPoint<T> const & getPointByOriginalI(size_t i) const {
        return points.at(i);
    }

protected:
    size_t K = 1;
    std::vector<KDPoint<T>> points;
    std::vector<size_t> indices;

private:
    /// It is necessary to use boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & K & points & indices;
    }
};

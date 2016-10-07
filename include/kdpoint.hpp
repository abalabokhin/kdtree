#pragma once

#include <boost/serialization/vector.hpp>

#include <stdexcept>
#include <vector>

template <typename T>
class KDPoint
{
public:
    /// Empty c-tor for serialization
    KDPoint() {}

    KDPoint(const std::vector<T> &aCoordinates)
        : coordinates(aCoordinates)
    {}
    T const & at(size_t i) const {
        return coordinates.at(i);
    }

    T squareDistanceToPoint(KDPoint<T> const & other) const {
        /// If checking the size is a bottle_neck, use something instead, e.g. assert.
        if (coordinates.size() != other.coordinates.size()) {
            /// TODO: implement better info in the exception, e.g print points.
            throw std::length_error("Size of points are not the same");
        }
        T distance{0};
        for (size_t i = 0; i < coordinates.size(); ++i) {
            auto diff = coordinates[i] - other.coordinates[i];
            distance += diff * diff;
        }
        return distance;
    }
private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & coordinates;
    }

    std::vector<T> coordinates;
};

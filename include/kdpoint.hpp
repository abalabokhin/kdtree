#pragma once

#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>

#include <stdexcept>
#include <vector>
#include <ostream>

template <typename T>
class KDPoint
{
public:
    /// Empty c-tor for serialization
    KDPoint() {}

    KDPoint(const std::vector<T> &aCoordinates)
        : coordinates(aCoordinates)
    {}

    /// Get the dimention of the point
    size_t size() const {
        return coordinates.size();
    }

    bool operator == (KDPoint<T> const & right) const {
        if (coordinates.size() != right.coordinates.size()) {
            throw std::length_error("size of points are not the same");
        }
        for (size_t i = 0; i < coordinates.size(); ++i) {
            if (coordinates[i] != right.coordinates[i])
                return false;
        }
        return true;
    }

    /// Get coordiante value at ith coordinate
    T const & at(size_t i) const {
        return coordinates.at(i);
    }

    T squareDistanceToPoint(KDPoint<T> const & other) const {
        /// If checking the size is a bottle_neck, use something instead, e.g. assert.
        if (coordinates.size() != other.coordinates.size()) {
            throw std::length_error("size of points are not the same");
        }
        T distance{0};
        for (size_t i = 0; i < coordinates.size(); ++i) {
            auto diff = coordinates[i] - other.coordinates[i];
            distance += diff * diff;
        }
        return distance;
    }
private:
    /// Boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & coordinates;
    }

    std::vector<T> coordinates;

    /// To use in boost tests
    template <typename K>
    friend std::ostream & operator<<(std::ostream& os, const KDPoint<K> & p);
};

template <typename T>
std::ostream & operator<<(std::ostream& os, const KDPoint<T> & p) {
    os << '{';
    for (auto & c : p.coordinates) {
        os << c << " ";
    }
    os << '}' << std::endl;
    return os;
}

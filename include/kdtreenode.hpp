#pragma once

#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

#include <memory>

/// Interface for KDTReeNode. It has two children right now: leaf node and intermediate node.
class IKDTreeNode
{
public:
    virtual ~IKDTreeNode() {}

private:
    /// Boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(IKDTreeNode)

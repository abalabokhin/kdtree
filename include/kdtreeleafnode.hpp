#pragma once

#include <kdtreenode.hpp>

#include <boost/serialization/base_object.hpp>

/// leaf node of kd-tree, keep left and right indexis in points storage.
class KDTreeLeafNode : public IKDTreeNode
{
public:
    /// Empty c-tor for serialization
    KDTreeLeafNode() {}

    KDTreeLeafNode(size_t aLeftPointsI, size_t aRightPointsI)
        : leftPointsI(aLeftPointsI), rightPointsI(aRightPointsI)
    {}

    size_t getLeftI() { return leftPointsI; }
    size_t getRightI() { return rightPointsI; }

private:
    /// Boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        boost::serialization::void_cast_register<KDTreeLeafNode, IKDTreeNode>();
        boost::serialization::base_object<IKDTreeNode>(*this);
        ar & leftPointsI & rightPointsI;
    }

    size_t leftPointsI;
    size_t rightPointsI;
};

BOOST_CLASS_EXPORT(KDTreeLeafNode)

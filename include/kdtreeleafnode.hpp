#pragma once

#include<kdtreenode.hpp>

/// if node is a leaf
class KDTreeLeafNode : public IKDTreeNode
{
public:
    /// Empty c-tor for serialization
    KDTreeLeafNode() {}

    KDTreeLeafNode(size_t aLeftPointsIndecisI, size_t aRightPointsIndecisI)
        : leftPointsIndecisI(aLeftPointsIndecisI), rightPointsIndecisI(aRightPointsIndecisI)
    {}

    size_t getLeftI() { return leftPointsIndecisI; }
    size_t getRightI() { return rightPointsIndecisI; }

private:
    /// It is necessary to use boost serialization
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        boost::serialization::void_cast_register<KDTreeLeafNode, IKDTreeNode>();
        boost::serialization::base_object<IKDTreeNode>(*this);
        ar & leftPointsIndecisI & rightPointsIndecisI;
    }

    size_t leftPointsIndecisI;
    size_t rightPointsIndecisI;
};

BOOST_CLASS_EXPORT(KDTreeLeafNode)

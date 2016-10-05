#pragma once

#include <cstddef>
#include <array>

template <typename T>
class KDtree {
public:
    KDtree(size_t aK)
        :   k(aK)
    {
        std::array<float, 3> pam;

    }




private:
    size_t k;

};

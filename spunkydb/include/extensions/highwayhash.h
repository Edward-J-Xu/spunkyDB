#ifndef HIGHWAYHASH_H
#define HIGHWAYHASH_H

#include <string>
#include "highwayhash/highwayhash.h"

namespace spunkydbext {

using namespace highwayhash;

class HighwayHash {
public:
    HighwayHash();
    HighwayHash(std::uint64_t s1, std::uint64_t s2, std::uint64_t s3, std::uint64_t s4);
    ~HighwayHash();

    std::size_t operator() (const std::string& s) const ;
private:
    const HHKey m_key HH_ALIGNAS(64);
    HighwayHashCatT<HH_TARGET>* m_hh;
    HHResult64* m_result;
};

}

#endif

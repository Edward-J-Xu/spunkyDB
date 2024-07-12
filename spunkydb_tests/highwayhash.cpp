#include "catch.hpp"
#include "../spunkydb/spunkydb.h"
#include "../spunkydb/spunkydbext.h"
#include "highwayhash/highwayhash.h"

#include <unordered_map>

TEST_CASE("Test unordered_map custom hashing algorithms","[set,get]") {
    SECTION("Produces known expected value for known input") {
        spunkydbext::HighwayHash h;
        std::string text = "Known";
        std::size_t r = h(text);

        using namespace highwayhash;
        const HHKey key HH_ALIGNAS(64) = {1, 2, 3, 4};
        HHResult64 result;  // or HHResult128 or HHResult256
        HHStateT<HH_TARGET> state(key);
        HighwayHashT(&state, text.c_str(), text.length(), &result);

        REQUIRE(r == result);
    }

    SECTION("Previous hash doesn't affect next hash result") {
        spunkydbext::HighwayHash hfirst;
        std::size_t rfirst = hfirst("OtherThing");

        spunkydbext::HighwayHash hsecond;
        std::size_t rsecond = hsecond("OtherThing");
        std::size_t rsecond1 = hsecond("Known");

        REQUIRE(rfirst == rsecond);
        REQUIRE(rfirst != rsecond1);
    }

    SECTION("Different seed keys produce different results for the same input") {
        spunkydbext::HighwayHash h1{1, 2, 3, 4};
        std::size_t r1 = h1("AThing");

        spunkydbext::HighwayHash h2{5, 6, 7, 8};
        std::size_t r2 = h2("AThing");

        REQUIRE(r1 != r2);
    }

    SECTION("Unordered map works as expected with custom hash") {
        std::unordered_map<std::string, std::string, spunkydbext::HighwayHash> m;
        std::string key("A very sensible key name");
        std::string value("A very valuable value");
        std::string k2("Some other key");
        std::string v2("Another value");
        m.emplace(key, value);
        m.emplace(k2, v2);

        REQUIRE(m[key] == value);
        REQUIRE(m[k2] == v2);
    }
}


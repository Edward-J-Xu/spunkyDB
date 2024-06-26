#include "catch.hpp"

#include "../spunkydb/spunkydb.h"

TEST_CASE("Store and retrieve a value", "[setKeyValue, getKeyValue]") {
    SECTION("Basic Set and Get") {
        std::string dbname("myemptydb");
        spunkydb::Database db(spunkydb::SpunkyDB::createEmptyDB(dbname));
        
        // We know we have been successful when:-
        // 1. The retrieved value is the same as the stored value
        std::string key("simplestring");
        std::string value("Some highly valuable value");
        db.setKeyValue(key, value);
        REQUIRE(value == db.getKeyValue(key));

        db.destroy();
    }
}

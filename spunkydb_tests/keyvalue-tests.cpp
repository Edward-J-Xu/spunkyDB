#include "catch.hpp"

#include "../spunkydb/spunkydb.h"

TEST_CASE("Store and retrieve a value", "[setKeyValue, getKeyValue]") {
    SECTION("Basic Set and Get") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname));
        
        // We know we have been successful when:-
        // 1. The retrieved value is the same as the stored value
        std::string key("simplestring");
        std::string value("Some highly valuable value");
        db->setKeyValue(key, value);
        REQUIRE(value == db->getKeyValue(key));

        std::string value2("Some highly valuable value number 2");
        db->setKeyValue(key, value2);
        REQUIRE(value2 == db->getKeyValue(key));

        db->destroy();
    }

    SECTION("Bucketed set and get") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname));
        
        // We know we have been successful when:-
        // 1. The retrieved value is the same as the stored value
        std::string key("simplestring");
        std::string value("Some highly valuable value");
        std::string bucket("bucket 1");
        db->setKeyValue(key, value, bucket);
        REQUIRE(value == db->getKeyValue(key, bucket));

        // Note bucket query is in the query-tests.cpp file

        db->destroy();
    }   
}

TEST_CASE("Store and retrieve a list", "setKeyValue(std::string, std::unordered_set<std::string>), getKeyValueSet") {
    SECTION("Basic List set and get") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname));

        // We know we have been successful when:-
        // 1. The retrieved value is the same as the stored value
        std::string key("simplestring");
        std::string val1("Some highly valuable value 1");
        std::string val2("Some highly valuable value 2");
        std::string val3("Some highly valuable value 3");
        std::unordered_set<std::string> valset;
        valset.insert(val1);
        valset.insert(val2);
        valset.insert(val3);
        db->setKeyValue(key, valset);
        auto result = db->getKeyValueSet(key);
        REQUIRE(result.get()->size() == 3);
        REQUIRE(result.get()->find(val1) != result.get()->end());
        REQUIRE(result.get()->find(val2) != result.get()->end());
        REQUIRE(result.get()->find(val3) != result.get()->end());

        db->destroy();
    }
}

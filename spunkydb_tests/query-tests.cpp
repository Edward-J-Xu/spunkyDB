#include "catch.hpp"
#include "../spunkydb/spunkydb.h"

#include <iostream>

TEST_CASE("Basic query tests","[query]") {
    SECTION("Bucketed set and get") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname));
        std::string key("simplestring");
        std::string value("Some highly valuable value");
        std::string key2("second string");
        std::string value2("second valuable value");
        std::string key3("third string");
        std::string value3("third valuable value");
        std::string key4("fourth string");
        std::string value4("fourth valuable value");
        std::string bucket("bucket 1");
        db->setKeyValue(key, value, bucket);
        db->setKeyValue(key2, value2);
        db->setKeyValue(key3, value3);
        db->setKeyValue(key4, value4, bucket);

        // Note set with bucket proven in keyvalue-tests.cpp

        // Now add the bucket query
        // second definition to ensure no weird pointer serialisation
        std::string queryBucket("bucket 1");
        spunkydb::BucketQuery bq(queryBucket);
        std::unique_ptr<spunkydb::IQueryResult> res = db->query(bq);
        std::unique_ptr<std::unordered_set<std::string>> recordKeys(res->recordKeys());
        
        for (auto it = recordKeys.get()->begin(); it != recordKeys.get()->end();it++) {
            INFO("  Key in bucket: " << *it);
        }
        INFO("  First key: " << *recordKeys.get()->begin());

        REQUIRE(recordKeys.get()->size() == 2);
        REQUIRE(recordKeys.get()->find(key) != recordKeys.get()->end());
        REQUIRE(recordKeys.get()->find(key4) != recordKeys.get()->end());

        db->destroy();
    }
}

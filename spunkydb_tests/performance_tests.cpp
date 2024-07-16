#define CATCH_CONFIG_ENABLE_BENCHMARKING 1
#include "catch.hpp"
#include "../spunkydb/spunkydb.h"
#include "../spunkydb/spunkydbext.h"

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>

TEST_CASE("Measure basic performance", "[setKeyValue, getKeyValue]") {
    SECTION("Store and retrieve 100 000 keys - Memory cached key-value store") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname));

        int total = 100000;

        // 1. Pre-generate 100 000 keys and values in memory so we don't skew the tests
        std::unordered_map<std::string, std::string> keyValues;
        long i = 0;
        std::cout << "Pre-generating key value pairs..." << std::endl;
        for (i = 0; i < total; i++) {
            keyValues.emplace(std::to_string(i), std::to_string(i)); // C++11 uses std::forward
        }
        std::cout << "Key size is max " << std::to_string(total - 1).length() << " bytes" << std::endl;

        // 2. Store 100 000 key-value pairs (no overlap)
        // Raw storage speed
        std::cout << "====== SET ======" << std::endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, it->second);
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "  " << keyValues.size() << " completed in "
                << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
                << " seconds" << std::endl;
        std::cout << "  "
                << (keyValues.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
                << " requests per second" << std::endl;
        std::cout << std::endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        std::string aString("blank");
        std::string& result(aString);
        std::cout << "====== GET ======" << std::endl;
        begin = std::chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            result = db->getKeyValue(it->first);
        }
        end = std::chrono::steady_clock::now();
        std::cout << "  " << keyValues.size() << " completed in "
                << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
                << " seconds" << std::endl;
        std::cout << "  "
                << (keyValues.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
                << " requests per second" << std::endl;
        
        // X. Retrieve 100 random keys
        //BENCHMARK("GET 100 random keys") {
        //  return db->getKeyValue(std::to_string(rand() % keyValues.size()));
        //};

        // 4. Retrive the same 100 000 key-value pairs
        // Retrive speed with warm cache (if any implemented)

        // 5. Store 50 000 key-value pair UPDATES (so half of the data is 'new') (first half of db)
        // So the performance of half of the key-value pairs may differ

        // 6. Retrieve the same 100 000 key-value pairs
        // Same as 4 if we have a 'write through cache', halfway between 4 and 3 otherwise

        // We know we have been successful when:-
        // We have min,max,var,total time for each group of operations
        // We have saved these results to a csv file for later comparison

        // 7. Destroy the database
        std::cout << "Tests complete" << std::endl;
        db->destroy();
    }

    SECTION("Store and Retrieve 100 000 keys - In-memory key-value store") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::KeyValueStore> memoryStore = std::make_unique<spunkydbext::MemoryKeyValueStore>();
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname, memoryStore));

        int total = 100000;
        
        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        std::unordered_map<std::string, std::string> keyValues;
        for (int i = 0; i < total; i++) {
            keyValues.emplace(std::to_string(i), std::to_string(i)); // C++11, uses std::forward
        }
        std::cout << "Key size is max " << std::to_string(total - 1).length() << " bytes" << std::endl;

        // 2. Store 100 000 key-value pairs (no overlap)
        // Raw storage speed
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, it->second);
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "  " << keyValues.size() << " completed in " 
            << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0) 
            << " seconds" << std::endl;
        std::cout << "  "
            << (keyValues.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
            << " requests per second" << std::endl;
        std::cout << std::endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        std::string aString("blank");
        std::string& result(aString);
        begin = std::chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            result = db->getKeyValue(it->first);
        }
        end = std::chrono::steady_clock::now();
        std::cout << "  " << keyValues.size() << " completed in "
            << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds" << std::endl;
        std::cout << "  "
            << (keyValues.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
            << " requests per second" << std::endl;
        
        // 7. Tear down
        db->destroy();
    }

    // Now do the same for pure disc backed storage
    SECTION("Store and Retrieve 100 000 keys - File based key-value store") {
        std::string dbname("myemptydb");
        std::string fullpath = ".spunkydb/" + dbname;
        std::unique_ptr<spunkydb::KeyValueStore> memoryStore = std::make_unique<spunkydbext::FileKeyValueStore>(fullpath);
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname, memoryStore));

        int total = 100000;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        std::unordered_map<std::string, std::string> keyValues;
        for (int i = 0; i < total; i++) {
            keyValues.emplace(std::to_string(i), std::to_string(i)); // C++11 uses std::forward
        }
        std::cout << "Key size is max " << std::to_string(total - 1).length() << " bytes" << std::endl;

        // 2. Store 100 000 key-value pairs (no overlap)
        // Raw storage speed
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, it->second);
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "  " << keyValues.size() << " completed in "
            << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds" << std::endl;
        std::cout << "  "
            << (keyValues.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
            << " requests per second" << std::endl;
        std::cout << std::endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        std::string aString("blank");
        std::string& result(aString);
        begin = std::chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            result = db->getKeyValue(it->first);
        }
        end = std::chrono::steady_clock::now();
        std::cout << "  " << keyValues.size() << " completed in "
            << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds" << std::endl;
        std::cout << "  "
            << (keyValues.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
            << " requests per second" << std::endl;

        // 4. Tear down
        db->destroy();
    }

    SECTION("Bucket query performance test - In-memory key-value store") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::KeyValueStore> memoryStore = std::make_unique<spunkydbext::MemoryKeyValueStore>();
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname, memoryStore));

        int total = 100000;
        int every = 1000;
        std::string bucket("my bucket");
        std::unordered_set<std::string> keysInBuckets;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        std::unordered_map<std::string, std::string> keyValues;
        for (int i = 0; i < total; i++) {
            keyValues.emplace(std::to_string(i), std::to_string(i)); // C++11 uses std::forward
            if (i % every == 0) {
                keysInBuckets.insert(std::to_string(i));
            }
        }
        std::cout << "Key size is max " << std::to_string(total - 1).length() << " bytes" << std::endl;

        // 2. Store 100 000 key-value pairs (no overlap)
        // Raw storage speed
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        long i = 0;
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            if (i % every == 0) {
                db->setKeyValue(it->first, it->second, bucket);
            } else {
                db->setKeyValue(it->first, it->second);
            }
            ++i;
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "  " << keyValues.size() << " completed in "
            << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds" << std::endl;
        std::cout << "  "
            << (keyValues.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
            << " requests per second" << std::endl;
        std::cout << std::endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        std::string aString("blank");
        std::string& result(aString);
        begin = std::chrono::steady_clock::now();
        for (auto it = keysInBuckets.begin(); it != keysInBuckets.end(); it++) {
            result = db->getKeyValue(*it);
        }
        end = std::chrono::steady_clock::now();
        auto getTimeMicro = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0);
        std::cout << "  " << keysInBuckets.size() << " completed in "
            << getTimeMicro
            << " seconds" << std::endl;
        std::cout << "  "
            << (keysInBuckets.size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
            << " requests per second" << std::endl;
        
        // 4. Query comparison
        spunkydb::BucketQuery bq(bucket);
        begin = std::chrono::steady_clock::now();
        std::unique_ptr<spunkydb::IQueryResult> res = db->query(bq);
        std::unique_ptr<std::unordered_set<std::string>> recordKeys(res->recordKeys());
        end = std::chrono::steady_clock::now();
        auto queryTimeMicro = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0);
        std::cout << "  " << recordKeys->size() << " completed in "
            << queryTimeMicro
            << " seconds" << std::endl;
        std::cout << "  "
            << (recordKeys->size() * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
            << " requests per second" << std::endl;
        REQUIRE(recordKeys->size() == keysInBuckets.size());
        REQUIRE(queryTimeMicro < getTimeMicro);

        // 5. Tear down
        db->destroy();
    }
}

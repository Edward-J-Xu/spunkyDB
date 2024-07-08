#include "catch.hpp"
#include "../spunkydb/spunkydb.h"

TEST_CASE("Measure basic performance", "[setKeyValue, getKeyValue]") {
    SECTION("Store and retrieve 1 000 000 keys - Memory cached key-value store") {
        std::string dbname("myemptydb");
        std::unique_ptr<spunkydb::IDatabase> db(spunkydb::SpunkyDB::createEmptyDB(dbname));

        // 1. Pre-generate 1 000 000 keys and values in memory so we don't skew the tests

        // 2. Store 1 000 000 key-value pairs (no overlap)
        // Raw storage speed

        // 3. Retrieve 100 000 key-value pairs (no overlap) (use 10 %)
        // Raw retrieval speed

        // 4. Retrive the same 100 000 key-value pairs
        // Retrive speed with warm cache (if any implemented)

        // 5. Store 500 000 key-value pair UPDATES (so half of the data is 'new') (first half of db)
        // So the performance of half of the key-value pairs may differ

        // 6. Retrieve the same 100 000 key-value pairs
        // Same as 4 if we have a 'write through cache', halfway between 4 and 3 otherwise

        // We know we have been successful when:-
        // We have min,max,var,total time for each group of operations
        // We have saved these results to a csv file for later comparison

        // 7. Destroy the database
        db->destroy();
    }
    // Now do the same for pure disc backed storage
}

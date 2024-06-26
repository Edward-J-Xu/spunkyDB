#include "catch.hpp"
#include "tests.h"
#include "../spunkyDB/spunkydb.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("Create an empty database", "[createEmptyDB]") {
    SECTION("Default settings") {
        std::string dbname = "myemptydb";
        spunkydb::Database db(spunkydb::SpunkyDB::createEmptyDB(dbname));

        // We know we have been successful when:-
        // 1. We have a valid database reference returned
        //   - No test -> The above would have errored
        // 2. The DB has a folder that exists on the file system
        REQUIRE(fs::is_directory(fs::status(db.getDirectory())));

        // 3. The database folder is empty (no database files yet)
        const auto& p = fs::directory_iterator(db.getDirectory());
        REQUIRE(p == end(p));

        db.destroy();
        REQUIRE(!fs::exists(fs::status(db.getDirectory())));
    }
}

TEST_CASE("Load an existing database", "[loadDB]") {
    SECTION("Default settings") {
        std::string dbname = "myemptydb";
        spunkydb::Database db(spunkydb::SpunkyDB::createEmptyDB(dbname));
        
        spunkydb::Database db2(spunkydb::SpunkyDB::loadDB(dbname));

        // We know we have been successful when:-
        // 1. We have a valid database reference returned
        //   - No test -> The above would have errored
        // 2. The DB has a folder that exists on the file system
        REQUIRE(fs::is_directory(fs::status(db2.getDirectory())));

        // 3. The database folder is empty (no database files yet)
        const auto& p = fs::directory_iterator(db2.getDirectory());
        REQUIRE(p == end(p));

        db2.destroy();
        REQUIRE(!fs::exists(fs::status(db2.getDirectory())));
    }
}


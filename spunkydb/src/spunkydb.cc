#include "spunkydb.h"

using namespace spunkydb;

SpunkyDB::SpunkyDB() {
    ;
}

Database SpunkyDB::createEmptyDB(std::string &dbname) {
    return Database::createEmpty(dbname);
}

Database SpunkyDB::loadDB(std::string &dbname) {
    return Database::load(dbname);
}


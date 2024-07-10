#include "spunkydb.h"
#include "extensions/extdatabase.h"

using namespace spunkydb;
using namespace spunkydbext;


SpunkyDB::SpunkyDB() {
    ;
}

std::unique_ptr<IDatabase> SpunkyDB::createEmptyDB(std::string &dbname) {
    return EmbeddedDatabase::createEmpty(dbname);
}

std::unique_ptr<IDatabase> SpunkyDB::createEmptyDB(std::string &dbname, std::unique_ptr<KeyValueStore>& kvStore) {
  return EmbeddedDatabase::createEmpty(dbname,kvStore);
}

std::unique_ptr<IDatabase> SpunkyDB::loadDB(std::string &dbname) {
    return EmbeddedDatabase::load(dbname);
}


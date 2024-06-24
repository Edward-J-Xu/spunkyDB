#ifndef SPUNKYDB_H
#define SPUNKYDB_H

#include <string>

#include "database.h"

class SpunkyDB {
public:
    SpunkyDB();

    static Database createEmptyDB(std::string& dbname);
    static Database loadDB(std::string& dbname);
};

#endif

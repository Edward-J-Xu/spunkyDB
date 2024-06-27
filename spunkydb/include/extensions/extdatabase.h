#ifndef EXTDATABASE_H
#define EXTDATABASE_H

#include "../database.h"
#include <memory>

namespace spunkydbext {

using namespace spunkydb;

class EmbeddedDatabase : public IDatabase {
public:
    EmbeddedDatabase(std::string dbname, std::string fullpath);
    ~EmbeddedDatabase();

    std::string getDirectory(void);

    // Key-Value Interface
    void setKeyValue(std::string key, std::string value);
    std::string getKeyValue(std::string key);

    // Management functions
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
    static std::unique_ptr<IDatabase> load(std::string dbname);
    void destroy(void);

    class Impl;
private:
    std::unique_ptr<Impl> mImpl;
};

}

# endif // EXTDATABASE_H

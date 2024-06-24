#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database {
public:
    Database(std::string dbname, std::string dbpath);

    // Management Functions
    static Database createEmpty(std::string dbname);
    static Database load(std::string dbname);

protected:
    std::string m_dbname;
    std::string m_dbpath;
};

#endif

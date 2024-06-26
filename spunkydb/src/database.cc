#include "database.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace spunkydb;

namespace fs = std::filesystem;

Database::Database(std::string dbname, std::string dbpath) : m_dbname(dbname), m_dbpath(dbpath) {}

// Management functions
Database Database::createEmpty(std::string dbname) {
    std::string basedir(".spunkydb");
    if (!fs::exists(basedir)) {
        fs::create_directory(basedir);
    }

    std::string dbfolder(basedir + "/" + dbname);
    if (!fs::exists(dbfolder)) {
        fs::create_directory(dbfolder);
    }

    return Database(dbname, dbfolder);
}

Database Database::load(std::string dbname) {
    std::string basedir(".spunkydb");
    std::string dbfolder(basedir + "/" + dbname);
    return Database(dbname, dbfolder);
}

void Database::destroy() {
    if (fs::exists(m_dbpath)) {
        fs::remove_all(m_dbpath);
    }
}


// Instance User Functions

std::string Database::getDirectory() {
    return m_dbpath;
}

void Database::setKeyValue(std::string key, std::string value) {
    std::ofstream os;
    os.open(m_dbpath + "/" + key + "_string.kv", std::ios::out | std::ios::trunc);
    os << value;
    os.close();
}

std::string Database::getKeyValue(std::string key) {
    std::ifstream is(m_dbpath + "/" + key + "_string.kv");
    std::string value;

    is.seekg(0, std::ios::end);
    value.reserve(is.tellg());
    is.seekg(0, std::ios::beg);

    value.assign((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    return value;
}

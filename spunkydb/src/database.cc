#include "database.h"
#include "extensions/extdatabase.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace spunkydb;
using namespace spunkydbext;

namespace fs = std::filesystem;

// 'Hidden' Database::Impl class here
class EmbeddedDatabase::Impl : public IDatabase {
public:
    Impl(std::string dbname, std::string dbpath);
    ~Impl();

    std::string getDirectory();

    // Key-Value Interface
    void setKeyValue(std::string key, std::string value);
    std::string getKeyValue(std::string key);

    // Management functions
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
    static std::unique_ptr<IDatabase> load(std::string dbname);
    void destroy();

private:
    std::string m_dbname;
    std::string m_dbpath;
    // 1. add an in-memory map here
};

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string dbpath) : m_dbname(dbname), m_dbpath(dbpath) {
    // 2. load all keys from disc to our in-memory map here
}

EmbeddedDatabase::Impl::~Impl() {
    // Z. [Optional] Flush the latest known state to disk here
}

// Management functions
const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::createEmpty(std::string dbname) {
    std::string basedir(".spunkydb");
    if (!fs::exists(basedir)) {
        fs::create_directory(basedir);
    }

    std::string dbfolder(basedir + "/" + dbname);
    if (!fs::exists(dbfolder)) {
        fs::create_directory(dbfolder);
    }

    return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::load(std::string dbname) {
    std::string basedir(".spunkydb");
    std::string dbfolder(basedir + "/" + dbname);
    return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder);
}

void EmbeddedDatabase::Impl::destroy() {
    if (fs::exists(m_dbpath)) {
        fs::remove_all(m_dbpath);
    }
    // 3. Don't forget to clear the in-memory map here
}


// Instance User Functions

std::string EmbeddedDatabase::Impl::getDirectory() {
    return m_dbpath;
}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::string value) {
    std::ofstream os;
    os.open(m_dbpath + "/" + key + "_string.kv", std::ios::out | std::ios::trunc);
    os << value;
    os.close();
    // 4. Also write to our in-memory map here
    // Storage Mechanism Paradigm: Strongly Consistent
    // If we didn't flush it here but say every minute, it is Eventually Consistent
}

std::string EmbeddedDatabase::Impl::getKeyValue(std::string key) {
    // 5. Only ever read from our in-memory map here
    std::ifstream is(m_dbpath + "/" + key + "_string.kv");
    std::string value;

    is.seekg(0, std::ios::end);
    value.reserve(is.tellg());
    is.seekg(0, std::ios::beg);

    value.assign((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    return value;
}


// High level Database client API implementation below


// Embedded Database
EmbeddedDatabase::EmbeddedDatabase(std::string dbname)
    : m_impl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath)) {
    ;
}

EmbeddedDatabase::~EmbeddedDatabase() {
    ;
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::createEmpty(std::string dbname) {
    return EmbeddedDatabase::Impl::createEmpty(dbname);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::load(std::string dbname) {
    return EmbeddedDatabase::Impl::load(dbname);
}

void EmbeddedDatabase::destroy() {
    m_impl->destroy();
}

// Instances User Functions

std::string EmbeddedDatabase::getDirectory() {
    return m_impl->getDirectory();
}

void EmbeddedDatabase::setKeyValue(std::string key, std::string value) {
    m_impl->setKeyValue(key, value);
}

std::string EmbeddedDatabase::getKeyValue(std::string key) {
    return m_impl->getKeyValue(key);
}


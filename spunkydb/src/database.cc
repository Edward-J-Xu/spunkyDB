#include "database.h"
#include "extensions/extdatabase.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>

using namespace spunkydb;
using namespace spunkydbext;

namespace fs = std::filesystem;

// 'Hidden' Database::Impl class here
class EmbeddedDatabase::Impl : public IDatabase {
public:
    Impl(std::string dbname, std::string dbpath);
    Impl(std::string dbname, std::string fullpath, std::unique_ptr<KeyValueStore>& kvStore);
    ~Impl();

    std::string getDirectory();

    // Key-Value Interface
    void setKeyValue(std::string key, std::string value);
    std::string getKeyValue(std::string key);

    // Management functions
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname, std::unique_ptr<KeyValueStore>& kvStore);
    static std::unique_ptr<IDatabase> load(std::string dbname);
    void destroy();

private:
    std::string m_dbname;
    std::string m_dbpath;
    std::unique_ptr<KeyValueStore> m_keyValueStore;
};

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string dbpath) : m_dbname(dbname), m_dbpath(dbpath) {
    // Explicitly specify base type so it matches the make_unique expected class (KeyValueStore)
    std::unique_ptr<KeyValueStore> fileStore = std::make_unique<MemoryKeyValueStore> (dbpath);
    std::unique_ptr<KeyValueStore> memoryStore = std::make_unique<MemoryKeyValueStore> (fileStore);
    m_keyValueStore = std::move(memoryStore);
}

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string dbpath, std::unique_ptr<KeyValueStore>& kvStore) 
    : m_dbname(dbname), m_dbpath(dbpath), m_keyValueStore(kvStore.release()) {
    ;
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

    return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::createEmpty(std::string dbname, std::unique_ptr<KeyValueStore>& kvStore) {
    std::string basedir(".spunkydb");
    if (!fs::exists(basedir)) {
        fs::create_directory(basedir);
    }
    std::string dbfolder(basedir + "/" + dbname);
    return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder, kvStore);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::load(std::string dbname) {
    std::string basedir(".spunkydb");
    std::string dbfolder(basedir + "/" + dbname);
    return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder);
}

void EmbeddedDatabase::Impl::destroy() {
    m_keyValueStore->clear();
}


// Instance User Functions

std::string EmbeddedDatabase::Impl::getDirectory() {
    return m_dbpath;
}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::string value) {
    m_keyValueStore->setKeyValue(key, value);
}

std::string EmbeddedDatabase::Impl::getKeyValue(std::string key) {
    return m_keyValueStore->getKeyValue(key);
}


// High level Database client API implementation below


// Embedded Database
EmbeddedDatabase::EmbeddedDatabase(std::string dbname)
    : m_impl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath)) {
    ;
}

EmbeddedDatabase::EmbeddedDatabase(std::string dbname, std::string fullpath,
                                   std::unique_ptr<KeyValueStore>& kvStore)
    : mImpl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath, kvStore)) {
    ;
}

EmbeddedDatabase::~EmbeddedDatabase() {
    ;
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::createEmpty(std::string dbname) {
    return EmbeddedDatabase::Impl::createEmpty(dbname);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::createEmpty(std::string dbname, std::unique_ptr<KeyValueStore>& kvStore) {
  return EmbeddedDatabase::Impl::createEmpty(dbname, kvStore);
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


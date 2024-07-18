#include "database.h"
#include "query.h"
#include "extensions/extquery.h"
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
    void setKeyValue(std::string key, std::string value, std::string bucket);
    void setKeyValue(std::string key, std::unordered_set<std::string> value);

    std::string getKeyValue(std::string key);

    std::unique_ptr<std::unordered_set<std::string>> getKeyValueSet(std::string key);

    // Query functions
    std::unique_ptr<IQueryResult> query(Query& query) const;
    std::unique_ptr<IQueryResult> query(BucketQuery& query) const;
    void indexForBucket(std::string key, std::string bucket);

    // Management functions
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname, std::unique_ptr<KeyValueStore>& kvStore);
    static std::unique_ptr<IDatabase> load(std::string dbname);
    void destroy();

    std::string m_dbname;
    std::string m_dbpath;
    std::unique_ptr<KeyValueStore> m_keyValueStore;
    std::unique_ptr<KeyValueStore> m_indexStore;
};

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string dbpath) : m_dbname(dbname), m_dbpath(dbpath) {
    // Explicitly specify base type so it matches the make_unique expected class (KeyValueStore)
    std::unique_ptr<KeyValueStore> fileStore = std::make_unique<MemoryKeyValueStore> (dbpath);
    std::unique_ptr<KeyValueStore> memoryStore = std::make_unique<MemoryKeyValueStore> (fileStore);
    m_keyValueStore = std::move(memoryStore);

    // Explicitly specify base type so it matches the make_unique expected class (KeyValueStore)
    std::unique_ptr<KeyValueStore> fileIndexStore = std::make_unique<FileKeyValueStore>(dbpath + "/.indexes");
    std::unique_ptr<KeyValueStore> memIndexStore = std::make_unique<MemoryKeyValueStore>(fileIndexStore);
    m_indexStore = std::move(memIndexStore);
}

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string dbpath, std::unique_ptr<KeyValueStore>& kvStore) 
    : m_dbname(dbname), m_dbpath(dbpath), m_keyValueStore(kvStore.release()) {
    // Explicitly specify base type so it matches the make_unique expected class (KeyValueStore)
    std::unique_ptr<KeyValueStore> fileIndexStore = std::make_unique<FileKeyValueStore>(dbpath + "/.indexes");
    std::unique_ptr<KeyValueStore> memIndexStore = std::make_unique<MemoryKeyValueStore>(fileIndexStore);
    m_indexStore = std::move(memIndexStore);
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

std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::load(std::string dbname) {
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

// QUESTION: What is the above storage mechanism paradigm?
//   ANSWER: Strongly Consistent (we may lose some data due to fsync delay
//           if our app crashes / machine powers off)
// QUESTION: What would it be called if we didn't flush to disc here?
//   ANSWER: Eventually consistent (E.g. if we flushed data every minute)
void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::string value) {
    m_keyValueStore->setKeyValue(key, value);
}

std::string EmbeddedDatabase::Impl::getKeyValue(std::string key) {
    return m_keyValueStore->getKeyValue(key);
}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::string value, std::string bucket) {
    setKeyValue(key, value);
    indexForBucket(key, bucket);
}

void EmbeddedDatabase::Impl::indexForBucket(std::string key, std::string bucket) {
    // Add to bucket index
    std::string idxKey("bucket::" + bucket);
    // query the key index
    std::unique_ptr<std::unordered_set<std::string>> recordKeys(m_indexStore->getKeyValueSet(idxKey));
    recordKeys.get()->insert(key);
    m_indexStore->setKeyValue(idxKey, *recordKeys.release()); // TODO do we need this? Yes - may not be in memory store
    // TODO replace the above with appendKeyValueSet(key, value)
}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::unordered_set<std::string> value) {
    m_keyValueStore->setKeyValue(key, value);
}

std::unique_ptr<std::unordered_set<std::string>> EmbeddedDatabase::Impl::getKeyValueSet(std::string key) {
    return m_keyValueStore->getKeyValueSet(key);
}

// Query functions

std::unique_ptr<IQueryResult> EmbeddedDatabase::Impl::query(Query& q) const {
    // return std::make_unique<DefaultQueryResult>(); // no results
    // Query is abstract, so try overloading here
    return query(static_cast<decltype(q)>(q));
}

std::unique_ptr<IQueryResult> EmbeddedDatabase::Impl::query(BucketQuery& query) const {
    // Bucket query
    // construct a name for our key index
    std::string idxKey("bucket::" + query.bucket());
    // query the key index

    std::unique_ptr<IQueryResult> r = std::make_unique<DefaultQueryResult>(m_indexStore->getKeyValueSet(idxKey));
    //std::cout << "EDB::Impl:query result size: " << r.get()->recordKeys()->size() << std::endl;
    return std::move(r);
}

// High level Database client API implementation below


// Embedded Database
EmbeddedDatabase::EmbeddedDatabase(std::string dbname, std::string fullpath)
    : m_impl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath)) {
    ;
}

EmbeddedDatabase::EmbeddedDatabase(std::string dbname, std::string fullpath,
                                   std::unique_ptr<KeyValueStore>& kvStore)
    : m_impl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath, kvStore)) {
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

std::unique_ptr<IDatabase> EmbeddedDatabase::load(std::string dbname) {
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

void EmbeddedDatabase::setKeyValue(std::string key, std::string value, std::string bucket) {
    m_impl->setKeyValue(key, value, bucket);
}

void EmbeddedDatabase::setKeyValue(std::string key, std::unordered_set<std::string> value) {
    m_impl->setKeyValue(key, value);
}

std::string EmbeddedDatabase::getKeyValue(std::string key) {
    return m_impl->getKeyValue(key);
}

std::unique_ptr<std::unordered_set<std::string>> EmbeddedDatabase::getKeyValueSet(std::string key) {
    return m_impl->getKeyValueSet(key);
}

// MARK: Query functions
std::unique_ptr<IQueryResult> EmbeddedDatabase::query(Query& query) const {
    return m_impl->query(query);
}

std::unique_ptr<IQueryResult> EmbeddedDatabase::query(BucketQuery& query) const {
    return m_impl->query(query);
}

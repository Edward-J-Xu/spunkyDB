#ifndef EXTDATABASE_H
#define EXTDATABASE_H

#include "../database.h"
#include <memory>
#include <functional>

namespace spunkydbext {

using namespace spunkydb;

// Ephemeral
class MemoryKeyValueStore : public KeyValueStore {
public:
    MemoryKeyValueStore();
    MemoryKeyValueStore(std::unique_ptr<KeyValueStore>& toCache);
    ~MemoryKeyValueStore();

    // Key-Value User Functions
    void setKeyValue(std::string key, std::string value);
    void setKeyValue(std::string key, std::string value, std::string bucket);
    void setKeyValue(std::string key, std::unordered_set<std::string> value);

    std::string getKeyValue(std::string key);
    std::unique_ptr<std::unordered_set<std::string>> getKeyValueSet(std::string key);

    // Key-Value Management Functions
    void loadKeysInto(std::function<void(std::string key, std::string value)> callback);
    void clear();

private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

class FileKeyValueStore : public KeyValueStore {
public:
    FileKeyValueStore(std::string fullpath);
    ~FileKeyValueStore();

    // Key-Value User Functions
    void setKeyValue(std::string key, std::string value);
    void setKeyValue(std::string key, std::string value, std::string bucket);
    void setKeyValue(std::string key, std::unordered_set<std::string> value);

    std::string getKeyValue(std::string key);
    std::unique_ptr<std::unordered_set<std::string>> getKeyValueSet(std::string key);

    void loadKeysInto(std::function<void(std::string key, std::string value)> callback);
    void clear();

private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

class EmbeddedDatabase : public IDatabase {
public:
    EmbeddedDatabase(std::string dbname, std::string fullpath);
    EmbeddedDatabase(std::string dbname, std::string fullpath,
                    std::unique_ptr<KeyValueStore>& kvStore);
    ~EmbeddedDatabase();

    std::string getDirectory(void);

    // Key-Value Interface
    void setKeyValue(std::string key, std::string value);
    void setKeyValue(std::string key, std::string value, std::string bucket);
    void setKeyValue(std::string key, std::unordered_set<std::string> value);

    std::string getKeyValue(std::string key);
    std::unique_ptr<std::unordered_set<std::string>> getKeyValueSet(std::string key);

    // Query records functions
    std::unique_ptr<IQueryResult> query(Query& query) const;
    std::unique_ptr<IQueryResult> query(BucketQuery& query) const;

    // Management functions
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
    static  const std::unique_ptr<IDatabase>    createEmpty(std::string dbname,std::unique_ptr<KeyValueStore>& kvStore);
    static std::unique_ptr<IDatabase> load(std::string dbname);
    void destroy(void);

    class Impl;
private:
    std::unique_ptr<Impl> mImpl;
};

}

# endif // EXTDATABASE_H

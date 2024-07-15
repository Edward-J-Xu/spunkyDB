#ifndef DATABASE_H
#define DATABASE_H

#include <string>

#include "query.h"

namespace spunkydb {

class Store {
public:
    Store() = default;
    virtual ~Store() = default;
};

using Set = std::unique_ptr<std::unordered_set<std::string>>;

class KeyValueStore : public Store {
public:
    KeyValueStore() = default;
    virtual ~KeyValueStore() = default;

    // Key-Value User Functions
    virtual void setKeyValue(std::string key, std::string value) = 0;
    virtual void setKeyValue(std::string key, std::string value, std::string bucket) = 0;
    virtual void setKeyValue(std::string key, std::unordered_set<std::string> value) = 0;

    virtual std::string getKeyValue(std::string key) = 0;
    virtual Set getKeyValueSet(std::string key) = 0;

    // Key-Value Management Functions
    virtual void loadKeysInto(std::function<void(std::string key, std::string value)> callback) = 0;
    virtual void clear() = 0;
};

using QueryResult = std::unique_ptr<IQueryResult>;

class IDatabase {
public:
    IDatabase() = default;

    virtual ~IDatabase() = default;

    virtual std::string getDirectory() = 0;
    // Key-Value Functions
    virtual void setKeyValue(std::string key, std::string value) = 0;
    virtual void setKeyValue(std::string key, std::string value, std::string bucket) = 0;
    virtual void setKeyValue(std::string key, std::unordered_set<std::string> value) = 0;

    virtual std::string getKeyValue(std::string key) = 0;
    virtual Set getKeyValueSet(std::string key) = 0;

    // Query records functions
    virtual QueryResult query(Query& query) const = 0;
    // TODO replace the below with just the generic polymorphic function
    virtual QueryResult query(BucketQuery& query) const = 0;


    // Management Functions
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
    static const std::unique_ptr<IDatabase> createEmpty(std::string dbname, std::unique_ptr<KeyValueStore>& kvStore);
    static std::unique_ptr<IDatabase> load(std::string dbname);
    
    virtual void destroy() = 0;
};

}


#endif

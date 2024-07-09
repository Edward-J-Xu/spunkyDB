#ifndef DATABASE_H
#define DATABASE_H

#include <string>

namespace spunkydb {

    class Store {
    public:
        Store() = default;
        virtual ~Store() = default;
    };

    class KeyValueStore : public Store {
    public:
        KeyValueStore() = default;
        virtual ~KeyValueStore() = default;

        // Key-Value User Functions
        virtual void setKeyValue(std::string key, std::string value) = 0;
        virtual std::string getKeyValue(std::string key) = 0;

        // Key-Value Management Functions
        virtual void loadKeysInto(std::function<void(std::string key, std::string value)> callback) = 0;
        virtual void clear() = 0;
    };

    class IDatabase {
    public:
        IDatabase() = default;

        virtual ~IDatabase() = default;

        virtual std::string getDirectory() = 0;
        // Key-Value Functions
        virtual void setKeyValue(std::string key, std::string value) = 0;
        virtual std::string getKeyValue(std::string key) = 0;

        // Management Functions
        static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
        static const std::unique_ptr<IDatabase> createEmpty(std::string dbname, std::unique_ptr<KeyValueStore>& kvStore);
        static std::unique_ptr<IDatabase> load(std::string dbname);
        
        virtual void destroy() = 0;
    };

}


#endif

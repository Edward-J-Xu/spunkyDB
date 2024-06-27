#ifndef DATABASE_H
#define DATABASE_H

#include <string>

namespace spunkydb {

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
        static std::unique_ptr<IDatabase> load(std::string dbname);
        
        virtual void destroy() = 0;
    };

}


#endif

#ifndef DATABASE_H
#define DATABASE_H

#include <string>

namespace spunkydb {

    class Database {
    public:
        Database(std::string dbname, std::string dbpath);

        std::string getDirectory();

        // Key-Value Functions
        void setKeyValue(std::string key, std::string value);
        std::string getKeyValue(std::string key);

        // Management Functions
        static Database createEmpty(std::string dbname);
        static Database load(std::string dbname);
        
        void destroy();

    protected:
        std::string m_dbname;
        std::string m_dbpath;
        // TODO std::pointer<DatabaseImpl> pImpl; // C++10 PIMPL IDIOM for forward binary compatibility
    };

}


#endif

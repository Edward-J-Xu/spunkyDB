#include "database.h"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

Database::Database(std::string dbname, std::string dbpath) : m_dbname(dbname), m_dbpath(dbpath) {}


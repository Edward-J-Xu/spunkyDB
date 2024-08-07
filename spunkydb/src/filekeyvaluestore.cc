#include "extensions/extdatabase.h"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace spunkydbext {

namespace fs = std::filesystem;

class FileKeyValueStore::Impl {
public:
    Impl(std::string fullpath);
    std::string m_fullpath;

private:
};

FileKeyValueStore::Impl::Impl(std::string fullpath) : m_fullpath(fullpath) {
    ;
}

FileKeyValueStore::FileKeyValueStore(std::string fullpath) : mImpl(std::make_unique<Impl>(fullpath)) {
    if (!fs::exists(fullpath)) {
        fs::create_directories(fullpath);
    }
}

FileKeyValueStore::~FileKeyValueStore() {
    ;
}

// Key-Value use cases
void
FileKeyValueStore::setKeyValue(std::string key, std::string value) {
    std::ofstream os;
    os.open(mImpl->m_fullpath + "/" + key + "_string.kv",
            std::ios::out | std::ios::trunc);
    os << value;
    os.close();
}

void FileKeyValueStore::setKeyValue(std::string key, std::unordered_set<std::string> value) {
    // store in _string_set.kl file elements_num<length,value...>...
    std::ofstream os;
    std::string fp(mImpl->m_fullpath + "/" + key + "_string_set.kv");
    os.open(fp, std::ios::out | std::ios::trunc);
    os << value.size() << std::endl;
    for (auto& val: value) {
        os << val.length() << std::endl;
        os << val.c_str() << std::endl;
    }
    os.close();
}

std::string FileKeyValueStore::getKeyValue(std::string key) {
    std::ifstream t(mImpl->m_fullpath + "/" + key + "_string.kv"); // DANGEROUS 
    std::string value;

    t.seekg(0, std::ios::end);
    value.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    value.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return value;
}

std::unique_ptr<std::unordered_set<std::string>> FileKeyValueStore::getKeyValueSet(std::string key) {
    // get from _string_set.kl file
    std::string fp(mImpl->m_fullpath + "/" + key + "_string_set.kv");
    if (!fs::exists(fp)) {
        return std::make_unique<std::unordered_set<std::string>>();
    }
    std::ifstream t(fp);
    std::unordered_set<std::string> values;
    std::string value;

    // read size first
    int entries;
    std::string eol; // eol can be multiple characters
    t >> entries;
    std::getline(t, eol);
    for (int i = 0; i < entries; i++) {
        // read length
        int sl;
        t >> sl;
        std::getline(t, eol);
        std::getline(t, value);
        values.insert(value);
    }
    return std::make_unique<std::unordered_set<std::string>>(values);
}

void FileKeyValueStore::loadKeysInto(std::function<void(std::string key, std::string value)> callback) {
    // load any files with .kv in their name
    for (auto& p : fs::directory_iterator(mImpl->m_fullpath)) {
        if (p.exists() && p.is_regular_file()) {
            // check if extension is .kv
            if (".kv" == p.path().extension()) {
                // If so, open file
                std::string keyWithString = p.path().filename();
                // ASSUMPTION always ends with _string.kv
                std::string key = keyWithString.substr(0, keyWithString.length() - 10); // DANGEROUS!!!

                std::ifstream t(p.path());
                std::string value;

                t.seekg(0, std::ios::end);
                value.reserve(t.tellg());
                t.seekg(0, std::ios::beg);

                value.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

                callback(key, value);
            }
        }
    }
}

void FileKeyValueStore::clear() {
    if (fs::exists(mImpl->m_fullpath)) {
        fs::remove_all(mImpl->m_fullpath);
    }
}

}

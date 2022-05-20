#ifndef __DATA_BASE_HPP__
#define __DATA_BASE_HPP__
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "../crawler/html.hpp"
namespace Amber{
enum Mode{
    Normal,//load all
    Search,//dont load the urs because not needed here to save ram
    CompressSearch//compress while loading to take less memory
};
class ReferenceDB{
    std::string m_filename;
    public:
    ReferenceDB(std::string);
    void write(std::unordered_map<std::string,uint64_t>);
    std::unordered_map<std::string,uint64_t> read();
};
class DataDB{
    std::string m_filename;
    public:
    DataDB(std::string);
    void write(std::unordered_map<std::string,Data>);
    std::unordered_map<std::string,Data> read(Mode m=Normal);
};
class CacheUrl{
    std::string m_filename;
    public:
    CacheUrl(std::string);
    void write(std::vector<std::string>);
    std::vector<std::string> read();
};
}
#endif

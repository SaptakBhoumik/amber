#ifndef __DATA_BASE_HPP__
#define __DATA_BASE_HPP__
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "../crawler/html.hpp"
namespace Amber{
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
    std::unordered_map<std::string,Data> read();
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
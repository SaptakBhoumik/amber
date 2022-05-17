#ifndef __CRAWLER_HPP__
#define __CRAWLER_HPP__
#include "html.hpp"
#include <string>
namespace Amber{
class Crawler{
    std::unordered_map<std::string,Data> m_result;
    std::unordered_map<std::string,uint64_t> m_referance;
    HTML m_html;
    std::vector<std::string> m_urls;
    public:
    Crawler(std::vector<std::string>,std::unordered_map<std::string,uint64_t>);
    void start();
    std::unordered_map<std::string,Data> get_data();
    std::unordered_map<std::string,uint64_t> get_referance();
    std::vector<std::string> get_urls();
};
}
#endif

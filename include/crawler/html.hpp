#ifndef AMBER_HTML
#define AMBER_HTML
#include "gumbo.h"
#include "../config.hpp"
#include <curl/curl.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
namespace Amber{
struct HTML_CODE{
    std::string url;
    std::string content;
};
struct Data {
    std::string title = "";
    std::string content = "";
    std::string original_url = "";
    std::unordered_set<std::string> url = {};
    //url,alt
    std::unordered_map<std::string,std::string> images;
};
class HTML{
    public:
    HTML()=default;
    std::vector<Data> get_data(std::vector<std::string>,long);//number of results
    private:
    HTML_CODE get_html(std::string);
    int mssleep(long miliseconds);
    Data get_data(GumboNode*,std::string);
    std::string full_url(std::string original,std::string part) ;
    
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static void _get_html(HTML* html,std::vector<Data>* content,std::string* url);
};
}
#endif
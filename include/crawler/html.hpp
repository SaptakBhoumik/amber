#ifndef AMBER_HTML
#define AMBER_HTML
#include <curl/curl.h>
#include <string>
#include <vector>
namespace Amber{
class HTML{
    public:
    HTML()=default;
    std::string get_html(std::string);
    std::vector<std::string> get_html(std::vector<std::string>);
};
}
#endif
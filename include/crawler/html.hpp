#ifndef AMBER_HTML
#define AMBER_HTML
#include <curl/curl.h>
#include <string>
#include <vector>
namespace Amber{
struct HTML_CODE{
    std::string url;
    std::string content;
};
class HTML{
    public:
    HTML()=default;
    HTML_CODE get_html(std::string);
    std::vector<HTML_CODE> get_html(std::vector<std::string>);
};
}
#endif
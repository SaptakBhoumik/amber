#ifndef AMBER_HTML
#define AMBER_HTML
#include <curl/curl.h>
#include <string>
namespace Amber{
class HTML{
    public:
    HTML()=default;
    std::string get_html(std::string url);
};
}
#endif
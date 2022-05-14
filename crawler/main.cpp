
#include "../include/crawler/html.hpp"
#include "../include/crawler/crawler.hpp"
using namespace Amber;
#include <chrono>
#include <iostream>
#include <sys/resource.h>
using namespace std::chrono;

int main() {
    struct rlimit R;
    getrlimit(RLIMIT_STACK, &R);
    R.rlim_cur = R.rlim_max;
    setrlimit(RLIMIT_STACK, &R);
    // auto x=HTML();
    // std::vector<std::string> visit;
    // for (size_t i=0; i<10;++i) {
    //     visit.push_back("https://en.wikipedia.org/wiki/Main_Page");
    // }
    // auto start = high_resolution_clock::now();
    // auto content=x.get_data(visit);
    // auto end = high_resolution_clock::now();
    // std::cout<<"Time taken to get html content in parallel is "<<duration_cast<milliseconds>(end -
    //                                                      start).count()<<"ms\n";
          
    // std::cout<<content.size()<<std::endl;
    // // std::cout<<content[0].content<<std::endl;
    // std::cout<<"TITLE = "<<content[0].title<<std::endl;
    // for(auto& x:content[0].images){
    //     std::cout<<x.first<<" = "<<x.second<<std::endl;
    // }
    auto x=Crawler("https://github.com",{});
    x.start();
    auto content=x.get_data();
    std::cout<<"No. of crawled url = " << content.size()<<std::endl;
}
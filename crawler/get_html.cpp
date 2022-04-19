#include <iostream>
#include <string>
#include "../include/crawler/html.hpp"
#define USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWfdgebKit/537.36 (KHTML, like Gecko) Chrome/99 Safari/537.36"
namespace Amber{
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    std::string* str=(std::string*)userp;
    str->reserve(realsize);
    auto c_str=(char*)contents;
    for(size_t i=0;i<realsize;i++){
        str->push_back(c_str[i]);
    }
    return realsize;
}
std::string HTML::get_html(std::string url) {
    std::string chunk;
    CURL *curl_handle=curl_easy_init();
    CURLcode res;
    if(curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT);

        res = curl_easy_perform(curl_handle);

        if(res != CURLE_OK) {
            std::cout<<"Can't get html content from "<<url<<"\n";
            fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
            return "";
        }
        curl_easy_cleanup(curl_handle);
    }
    else{
        std::cout<<"Error: Couldn't create a curl instance"<<std::endl;
        exit(1);
    }
    return chunk;
}
}
/*
using namespace Amber;
#include <chrono>
using namespace std::chrono;
#include <thread>
void get_html(HTML* html,std::string* content,std::string url){
    *content=html->get_html(url);
}
int main() {
    auto x=HTML();
    // std::cout<<x.get_html("http://127.0.0.1:5500/test.html")<<"\n";
    // std::cout<<x.get_html("http://127.0.0.1:5500/test2.html")<<"\n";
    std::string content1;
    std::string content2;
    std::string content3;
    auto start = high_resolution_clock::now();
    std::thread thread_obj1(get_html,&x,&content1,"google.com");
    std::thread thread_obj2(get_html,&x,&content2,"facebook.com");
    std::thread thread_obj3(get_html,&x,&content3,"wikipedia.com");
    thread_obj1.join();
    thread_obj2.join();
    thread_obj3.join();
    auto end = high_resolution_clock::now();
    std::cout<<"Time taken to get html content in parallel is "<<duration_cast<milliseconds>(end -
                                                         start).count()<<"ms\n";
    start = high_resolution_clock::now();
    x.get_html("google.com");
    x.get_html("facebook.com");
    x.get_html("wikipedia.com");
    end = high_resolution_clock::now();  
    std::cout<<"Time taken to get html content in sequence is "<<duration_cast<milliseconds>(end -
                                                         start).count()<<"ms\n";                                             
    // std::cout<<content1<<"\n";
    // std::cout<<content2<<"\n";
    // std::cout<<content3<<"\n";
}
*/
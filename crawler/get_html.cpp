#include <iostream>
#include <pthread.h>
#include <thread>
#include <string>
#include <ctime>
#include "../include/crawler/html.hpp"

#define USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWfdgebKit/537.36 (KHTML, like Gecko) Chrome/99 Safari/537.36"

namespace Amber{
int mssleep(long miliseconds)
{
   struct timespec rem;
   struct timespec req= {
       (int)(miliseconds / 1000),     /* secs (Must be Non-Negative) */ 
       (miliseconds % 1000) * 1000000 /* nano (Must be in range of 0 to 999999999) */ 
   };

   return nanosleep(&req , &rem);
}
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    if(contents!=NULL||userp!=NULL){
        std::string* str=(std::string*)userp;
        str->reserve(realsize);
        auto c_str=(char*)contents;
        for(size_t i=0;i<realsize;i++){
            str->push_back(c_str[i]);
        }
    }
    return realsize;
}

void _get_html(HTML* html,std::vector<HTML_CODE>* content,std::string* url){
    auto x=html->get_html(*url);
    if(x.content!=""){
        content->emplace_back(x);
    }
}

HTML_CODE HTML::get_html(std::string url) {
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
            return {"",""};
        }
        curl_easy_cleanup(curl_handle);
    }
    else{
        std::cout<<"Error: Couldn't create a curl instance"<<std::endl;
        return {"",""};
    }
    return {.url=url,.content=chunk};
}
std::vector<HTML_CODE> HTML::get_html(std::vector<std::string> urls) {
    std::vector<HTML_CODE> res;
    res.reserve(urls.size());
    std::vector<std::thread> threads; 
    threads.reserve(urls.size());
    for(size_t i=0;i<urls.size();++i){
        threads.emplace_back(std::thread(_get_html,this,&res,&urls[i]));
        mssleep(15);//needed so that the dns can handle it
    }
    for(auto& x:threads){
        x.join();
    }
    return res;
}
}
///*
using namespace Amber;
#include <chrono>
using namespace std::chrono;

int main() {
    auto x=HTML();
    std::vector<std::string> visit;
    for (size_t i=0; i<1000;++i) {
        visit.push_back("https://www.google.com/");
    }
    auto start = high_resolution_clock::now();
    auto content=x.get_html(visit);
                            // {
                            // "google.com","facebook.com","wikipedia.com",
                            // "https://www.visard.org","https://github.com/","https://github.com/notti/nocgo",
                            // "https://github.com/apachejuice/pretzel","https://github.com/ryaangu/poth",
                            // "https://github.com/peregrine-lang/Peregrine","https://godbolt.org/",
                            // "https://pypi.org/"}
                            // );
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
    std::cout<<content.size()<<std::endl;
}
//*/
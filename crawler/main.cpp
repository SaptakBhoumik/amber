
#include "../include/crawler/html.hpp"
#include "../include/crawler/crawler.hpp"
#include "../include/database/database.hpp"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <filesystem>
#include <sys/resource.h>
using std::filesystem::directory_iterator;
using namespace Amber;

int main(int argc, char const *argv[]) {
    struct rlimit R;
    getrlimit(RLIMIT_STACK, &R);
    R.rlim_cur = R.rlim_max;
    setrlimit(RLIMIT_STACK, &R);
    struct stat info;
    std::string url;
    std::string dataset;
    std::unordered_map<std::string,uint64_t> referance;
    if(argc!=2){
        std::cout<<"Enter the filepath to the dataset folder:- ";
        std::cin>>dataset;
    }
    else{
        dataset=argv[1];
    }
    if(stat(dataset.c_str(), &info) != 0 ){
        std::cout<<"The filepath is invalid.\n";
        return 1;
    }
    else if(!(info.st_mode & S_IFDIR)){
        std::cout<<"The filepath is not a directory.\n";
        return 1;
    }
    long total_file=0;
    for (const auto & file : directory_iterator(dataset+datadir)){
        total_file++;
    }
    if(total_file==0){
        std::cout<<"The dataset folder is empty. Please enter a url to start crawling from:- ";
        std::cin>>url;
    }
    else{
        CacheUrl cachedb(dataset+cache_url_file);
        auto cache=cachedb.read();
        if(cache.size()==0){
            std::cout<<"The cache is empty. Please enter a url to start crawling from:- ";
            std::cin>>url;
        }
        else{
            url=cache.back();
        }
        ReferenceDB rdb(dataset+referance_file);
        referance=rdb.read();
    }
    auto crawl=Crawler(url,referance);
    crawl.start();
    auto content=crawl.get_data();
    std::cout<<"No. of crawled url = " << content.size()<<std::endl;
    DataDB data(dataset+datadir+"/db"+std::to_string(total_file+1)+".db");
    data.write(content);
    ReferenceDB refdb(dataset+referance_file);
    refdb.write(crawl.get_referance());
    CacheUrl cachedb(dataset+cache_url_file);
    cachedb.write(crawl.get_urls());
}
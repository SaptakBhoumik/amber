#include <iostream>
#include <pthread.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "../include/crawler/html.hpp"
#include "../include/crawler/crawler.hpp"
namespace Amber{
Crawler::Crawler(std::vector<std::string> url,std::unordered_map<std::string,uint64_t> referance){
    m_referance=referance;
    m_urls=url;
}
void Crawler::start(){
    reset:{}
    auto data=m_html.get_data(m_urls,max_num_item-m_result.size());
    //TODO: take new input from user
    if (data.size()==0 && m_result.size()<max_num_item) {
        std::cout<<"No more data to crawl. Number of url done:- "<<m_result.size() <<"  . Enter a new url to crawl:- ";
        m_urls.clear();
        std::string url;
        std::cin>>url;
        m_urls.push_back(std::string(url));
        goto reset;
        return;
    }
    if((max_num_item-m_result.size())>=m_urls.size()){
        m_urls.clear();
    }
    else{
        auto x=max_num_item-m_result.size();
        for(size_t i=0;i<x;++i){
            m_urls.erase(m_urls.begin());
        }
    }
    for (auto& d:data){
        m_result[d.original_url]=d;
        for (auto& u:d.url){
            if (m_referance.find(u)==m_referance.end()){
                m_referance[u]=1;
                m_urls.push_back(u);
            }
            else{
                m_referance[u]++;
            }
        }
    }
    if(m_result.size()<=max_num_item){
        goto reset;
    }
}
std::unordered_map<std::string,Data> Crawler::get_data(){
    return m_result;
}
std::unordered_map<std::string,uint64_t> Crawler::get_referance(){
    return m_referance;
}
std::vector <std::string> Crawler::get_urls(){
    return m_urls;
}
}
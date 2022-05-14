#include <iostream>
#include <pthread.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "../include/crawler/html.hpp"
#include "../include/crawler/crawler.hpp"
namespace Amber{
Crawler::Crawler(std::string url,std::unordered_map<std::string,uint64_t> referance){
    m_url=url;
    m_referance=referance;
    m_urls.push_back(std::string(m_url));
}
void Crawler::start(){
    reset:{}
    auto data=m_html.get_data(m_urls,max_num_item-m_result.size());
    //TODO: take new input from user
    if (data.size()==0) {
        std::cout<<"No more data to crawl. Enter a new url to crawl:- ";
        m_urls.clear();
        std::cin>>m_url;
        m_urls.push_back(std::string(m_url));
        goto reset;
        return;
    }
    m_urls.clear();
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
    if(m_result.size()<max_num_item){
        goto reset;
    }
}
std::unordered_map<std::string,Data> Crawler::get_data(){
    return m_result;
}
}
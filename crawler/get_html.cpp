#include <iostream>
#include <thread>
#include <string>
#include <string.h>
#include <ctime>
#include "../include/crawler/html.hpp"

namespace Amber{
int HTML::mssleep(long miliseconds)
{
   struct timespec rem;
   struct timespec req= {
       (int)(miliseconds / 1000),     /* secs (Must be Non-Negative) */ 
       (miliseconds % 1000) * 1000000 /* nano (Must be in range of 0 to 999999999) */ 
   };

   return nanosleep(&req , &rem);
}
void HTML::_get_html(HTML* html,std::vector<Data>* content,std::string* url){
    auto x=html->get_html(*url);
    std::cout<<"Crawling: "<<*url<<std::endl;
    if(x.content!=""){
        GumboOutput *output = gumbo_parse(x.content.c_str());
        if(output!=NULL){
            auto _res=html->get_data(output->root,x.url);
            _res.original_url=x.url;
            content->push_back(_res);
            gumbo_destroy_output(&kGumboDefaultOptions, output);
        }
    }
}

size_t HTML::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    if(contents!=NULL||userp!=NULL){
        std::string* str=(std::string*)userp;
        str->reserve(realsize);
        auto c_str=(char*)contents;
        if(c_str!=NULL){
            for(size_t i=0;i<realsize;i++){
                str->push_back(c_str[i]);
            }
        }
    }
    return realsize;
}

HTML_CODE HTML::get_html(std::string url) {
    std::string chunk;
    CURL *curl_handle=curl_easy_init();
    CURLcode res;
    if(curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 0);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT);
        curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 5);
        res = curl_easy_perform(curl_handle);

        if(res != CURLE_OK) {
            std::cout<<"Can't get html content from "<<url<<"\n";
            fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
            return {"",""};
        }
        {
            char* ct=NULL;
            res = curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_TYPE, &ct);
            if(!res && ct) {
                if(std::string(ct).find("text/html") == std::string::npos){
                    return {};
                }
            }
        }
        curl_easy_cleanup(curl_handle);
    }
    else{
        std::cout<<"Error: Couldn't create a curl instance"<<std::endl;
        return {"",""};
    }
    return {.url=url,.content=chunk};
}
std::vector<Data> HTML::get_data(std::vector<std::string> urls,size_t num) {
    std::vector<Data> res;
    res.reserve(urls.size());
    std::vector<std::thread> threads; 
    for(size_t i=0;i<urls.size();++i){
        threads.emplace_back(std::thread(_get_html,this,&res,&urls[i]));
        mssleep(sleep_time_ms);//needed so that the dns can handle 
        num--;
        if(num<=0){
            break;
        }
    }
    for(auto& x:threads){
        x.join();
    }
    threads.clear();
    return res;
}

Data HTML::get_data(GumboNode* node,std::string original_url){
    if(node==NULL){
        return {};
    }
    if (node->type == GUMBO_NODE_TEXT) {
        Data contents;
        contents.content=std::string(node->v.text.text);
        return contents;
    } 
    else if(node->v.element.tag==GUMBO_TAG_BR){
        return {.content="\n"};; 
    }
    else if (node->v.element.tag == GUMBO_TAG_A) {
        // std::cout<<original_url<<" "<<node->v.element.attributes.length<<std::endl;
        GumboVector *children = &node->v.element.children;
        Data contents;
        if(node->v.element.attributes.length>10||node->v.element.attributes.length==0){
            return contents;
        }
        auto attr=node->v.element.attributes;
        if(gumbo_get_attribute(&attr, "download")!=NULL){
            return contents;
        }
        GumboAttribute* x=gumbo_get_attribute(&attr, "href");
        std::string url;
        if(x==NULL){
            url="";
        }
        else{
            url=full_url(original_url,x->value);
        }
        for (unsigned int i = 0; i < children->length; ++i) {
            const Data text = get_data((GumboNode *)children->data[i],original_url);
            if (text.url.size() > 0) {
                for (auto &i : text.url) {
                    if (!i.empty()) {
                        contents.url.insert(i);
                    }
                }
            }
            if (text.images.size() > 0) {
                for (auto &i : text.images) {
                    if (!i.first.empty()) {
                        if(i.first==url){
                            url="";
                        }
                        contents.images.insert(i);
                    }
                }
            }
        }
        if(!url.empty()){
            contents.url.insert(url);
        }
        return contents;
    }
    else if ( node->v.element.tag==GUMBO_TAG_IMG ){
        Data contents;
        //Bug in the gumbo library. So it cant be more than 3 attributes
        if(node->v.element.attributes.length>3||node->v.element.attributes.length==0){
            return contents;
        }
        auto x=gumbo_get_attribute(&node->v.element.attributes, "src");
        std::string url;
        if(x!=NULL){
            url=full_url(original_url,x->value);
        }

        auto y=gumbo_get_attribute(&node->v.element.attributes, "alt");
        std::string alt;
        if(y!=NULL){
            alt=y->value;
        }
        contents.images[url]=alt;
        return contents;
    } 
    else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE &&
               node->v.element.tag!=GUMBO_TAG_NOSCRIPT &&
               node->v.element.tag!=GUMBO_TAG_IFRAME &&
               node->v.element.tag!=GUMBO_TAG_EMBED &&
               node->v.element.tag!=GUMBO_TAG_OBJECT &&
               node->v.element.tag!=GUMBO_TAG_VIDEO &&
               node->v.element.tag!=GUMBO_TAG_AUDIO &&
               node->v.element.tag!=GUMBO_TAG_SOURCE && 
               node->v.element.tag!=GUMBO_TAG_TRACK && 
               node->v.element.tag!=GUMBO_TAG_SOURCE
               ) {
        Data contents;
        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const Data text = get_data((GumboNode *)children->data[i],original_url);
            if (text.url.size() > 0) {
                for (auto &i : text.url) {
                    if (!i.empty()) {
                        contents.url.insert(i);
                    }
                }
            }
            if (text.images.size() > 0) {
                for (auto &i : text.images) {
                    if (!i.first.empty()) {
                        contents.images.insert(i);
                    }
                }
            }
            if (node->v.element.tag == GUMBO_TAG_TITLE) {
                contents.title.append(text.content);
            } 
            else {
                contents.title.append(text.title);
                contents.content.append(text.content);
            }
        }
        return contents;
    } 
    else {
        return Data{};
    }
}
std::string HTML::full_url(std::string original,std::string part) {
    if(part.find('#') != std::string::npos ||  // location to a div element
        part.find('?') != std::string::npos || // just a quary
        part.find('@') != std::string::npos || // email
        part.find('%') != std::string::npos || // TODO: This types of url are causing problems
        part=="/"||part==original){
        return "";
    }
    else if(part[0]=='/' && part[1]=='/'){
        return "http:"+part;
    }
    else if(part[0]=='/'){
        if(original[original.size()-1]=='/'){
            return original.substr(0, original.size()-1)+part;
        }
        return original+part;
    }
    else if(part.find("https://") != std::string::npos || 
        part.find("http://") != std::string::npos){
        return part;
    }

    else{
        if(original[original.size()-1]=='/'){
            return original+part;
        }
        return original+"/"+part;
    }
}
}


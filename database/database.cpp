#include "../include/database/database.hpp"
#include <cstdint>
#include "../include/compress/compress.hpp"
#include <iostream>
namespace Amber{
ReferenceDB::ReferenceDB(std::string filename){
    m_filename=filename;
}
void ReferenceDB::write(std::unordered_map<std::string, uint64_t> reference){
    std::ofstream file(m_filename);
    if(!file){
        std::cout<<"Error creating file "<<m_filename<<std::endl;
        return;
    }
    auto size=htobe64(reference.size());
    file.write((char*)&size,sizeof(size));
    for (auto& r:reference){
        auto key_size=htobe64(r.first.size());
        file.write((char*)&key_size,sizeof(key_size));
        file.write(r.first.c_str(),r.first.size());
        auto value_size=htobe64(r.second);
        file.write((char*)&value_size,sizeof(value_size));
    }
    file.close();
}

std::unordered_map<std::string,uint64_t> ReferenceDB::read(){
    std::unordered_map<std::string,uint64_t> reference;
    std::ifstream file(m_filename);
    if(!file){
        std::cout<<"Error reading file "<<m_filename<<std::endl;
        return reference;
    }
    size_t size;
    file.read((char*)&size,sizeof(size));
    size=be64toh(size);
    for (uint64_t i=0;i<size;i++){
        size_t key_size;
        file.read((char*)&key_size,sizeof(key_size));
        key_size=be64toh(key_size);
        char key[key_size+1];
        file.read(key,key_size);
        key[key_size]='\0';
        size_t value_size;
        file.read((char*)&value_size,sizeof(value_size));
        value_size=be64toh(value_size);
        reference[key]=value_size;
    }
    file.close();
    return reference;
}
DataDB::DataDB(std::string filename){
    m_filename=filename;
}
void DataDB::write(std::unordered_map<std::string,Data> data){
    std::ofstream file(m_filename);
    if(!file){
        std::cout<<"Error creating file "<<m_filename<<std::endl;
        return;
    }
    auto size=htobe64(data.size());
    file.write((char*)&size,sizeof(size));
    for (auto& r:data){
        auto key_size=htobe64(r.first.size());
        file.write((char*)&key_size,sizeof(key_size));
        file.write(r.first.c_str(),r.first.size());
        
        auto second=r.second;
        auto title_size=htobe64(second.title.size());
        file.write((char*)&title_size,sizeof(title_size));
        file.write(second.title.c_str(),second.title.size());
        auto content_size=htobe64(second.content.size());
        file.write((char*)&content_size,sizeof(content_size));
        file.write(second.content.c_str(),second.content.size());
        auto link_size=htobe64(second.url.size());
        file.write((char*)&link_size,sizeof(link_size));
        for (auto& r:second.url){
            auto link_size=htobe64(r.size());
            file.write((char*)&link_size,sizeof(link_size));
            file.write(r.c_str(),r.size());
        }
        auto image_size=htobe64(second.images.size());
        file.write((char*)&image_size,sizeof(image_size));
        for (auto& r:second.images){
            auto image_url_size=htobe64(r.first.size());
            file.write((char*)&image_url_size,sizeof(image_size));
            file.write(r.first.c_str(),r.first.size());
            auto image_alt_size=htobe64(r.second.size());
            file.write((char*)&image_alt_size,sizeof(image_alt_size));
            file.write(r.second.c_str(),r.second.size());
        }
    }
}
std::unordered_map<std::string,Data> DataDB::read(Mode m){
    //I wont compress the keys of the map because they can cause some problems and i dont want to take that risk
    std::unordered_map<std::string,Data> res;
    std::ifstream file(m_filename);
    if(!file){
        std::cout<<"Error opening file "<<m_filename<<std::endl;
    }
    size_t size;
    file.read((char*)&size,sizeof(size));
    size=be64toh(size);
    for (size_t i=0;i<size;i++){
        Data data;
        size_t key_size;
        file.read((char*)&key_size,sizeof(key_size));
        key_size=be64toh(key_size);
        char key[key_size+1];
        file.read(key,key_size);
        key[key_size]='\0';
        data.original_url=key;

        size_t title_size;
        file.read((char*)&title_size,sizeof(title_size));
        title_size=be64toh(title_size);
        char title[title_size+1];
        file.read(title,title_size);
        title[title_size]='\0';
        if(m==CompressSearch){
            data.title=Compress::compress(title);
        }
        else{
            data.title=title;
        }

        size_t content_size;
        file.read((char*)&content_size,sizeof(content_size));
        content_size=be64toh(content_size);
        char content[content_size+1];
        file.read(content,content_size);
        content[content_size]='\0';
        if(m==CompressSearch){
            data.content=Compress::compress(content);
        }
        else{
            data.content=content;
        }
        size_t link_size;
        file.read((char*)&link_size,sizeof(link_size));
        link_size=be64toh(link_size);
        for (size_t i=0;i<link_size;i++){
            size_t link_size;
            file.read((char*)&link_size,sizeof(link_size));
            link_size=be64toh(link_size);
            char link[link_size+1];
            file.read(link,link_size);
            link[link_size]='\0';
            if(m==Normal){
                data.url.insert(link);
            }
        }

        size_t image_size;
        file.read((char*)&image_size,sizeof(image_size));
        image_size=be64toh(image_size);
        for (size_t i=0;i<image_size;i++){
            size_t image_url_size;
            file.read((char*)&image_url_size,sizeof(image_url_size));
            image_url_size=be64toh(image_url_size);
            char image_url[image_url_size+1];
            file.read(image_url,image_url_size);
            image_url[image_url_size]='\0';
            size_t image_alt_size;
            file.read((char*)&image_alt_size,sizeof(image_alt_size));
            image_alt_size=be64toh(image_alt_size);
            char image_alt[image_alt_size+1];
            file.read(image_alt,image_alt_size);
            image_alt[image_alt_size]='\0';
            if(m==CompressSearch){
                data.images[image_url]=Compress::compress(image_alt);
            }
            else{
                data.images[image_url]=image_alt;
            }
        }
        res[data.original_url]=data;
    }
    return res;
}
CacheUrl::CacheUrl(std::string filename){
    m_filename=filename;
}
void CacheUrl::write(std::vector<std::string> urls){
    std::ofstream file(m_filename);
    if(!file){
        std::cout<<"Error creating file "<<m_filename<<std::endl;
        return;
    }
    auto size=htobe64(urls.size());
    file.write((char*)&size,sizeof(size));
    for (auto& r:urls){
        auto url_size=htobe64(r.size());
        file.write((char*)&url_size,sizeof(url_size));
        file.write(r.c_str(),r.size());
    }
}
std::vector<std::string> CacheUrl::read(){
    std::vector<std::string> res;
    std::ifstream file(m_filename);
    if(!file){
        std::cout<<"Error opening file "<<m_filename<<std::endl;
    }
    size_t size;
    file.read((char*)&size,sizeof(size));
    size=be64toh(size);
    for (size_t i=0;i<size;i++){
        size_t url_size;
        file.read((char*)&url_size,sizeof(url_size));
        url_size=be64toh(url_size);
        char url[url_size+1];
        file.read(url,url_size);
        url[url_size]='\0';
        res.push_back(url);
    }
    return res;
}
}

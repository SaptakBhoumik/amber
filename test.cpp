#include "include/crawler/html.hpp"
#include "include/crawler/crawler.hpp"
#include "include/database/database.hpp"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <filesystem>
#include <sys/resource.h>
#include <unistd.h>
long get_mem(){
    rusage u;
    getrusage(RUSAGE_SELF,&u);
    return u.ru_maxrss;
}
using namespace Amber;
int main(){
    std::vector<std::unordered_map<std::string, Data>> data;
    int c=0;
    label:{}
    for(long i=1;i<345;++i){
        DataDB ref=DataDB("/home/saptak/Desktop/projects/amber/dataset/data/db"+std::to_string(i)+".db");
        data.push_back(ref.read(Search));
    }
    c++;
    if(c<5){
        goto label;
    }
    std::cout<<"current usage :- "<<get_mem()<<"\n";
    std::cout<<data.size()<<std::endl;
    std::cin.get();

}

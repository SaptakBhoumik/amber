#include <ctype.h>
#include <iostream>
#include "Amber/include/tokenizer/tokenizer.hpp"
#include "Amber/include/search/search.hpp"
#include "Amber/include/pagerank/pagerank.hpp"
#include <fstream>
#include <chrono>
#include <sstream>
#include <sys/resource.h>
#include <unistd.h>
long get_mem(){
    rusage u;
    getrusage(RUSAGE_SELF,&u);
    return u.ru_maxrss;
}
int main(){
    /*
    std::ifstream file("/home/saptak/Desktop/projects/test/test.txt");
    std::stringstream buf;
    buf << file.rdbuf();
    std::string para=buf.str();
    std::cout<<"Enter a quary:- ";
    std::string para2="";
    std::getline(std::cin,para2) ;//"who was the Serbian-American inventor";
    auto tok=Amber::Tokenizer::Tokenizer(para);
    tok.start();
    auto c=tok.getParagraph();
    auto tok2=Amber::Tokenizer::Tokenizer(para2);
    tok2.start();
    auto c2=tok2.getParagraph();
    // for(auto& x:c2.sentences){
    //     for(auto& y:x.words){
    //         std::cout<<y<<"\n";
    //     }
    // }
    //measure time
    auto start=std::chrono::high_resolution_clock::now();
    auto s=Amber::Search::Search(c2,c,0.80,10000);
    s.start();
    auto r=s.get_result();
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout<<"Time taken to show the results: "<<duration.count()<<" milliseconds\n";
    std::cout<<"current usage :- "<<get_mem()/1000.0<<"mb"<<"\n";
    std::cout<<r[0].sentence.sentence<<std::endl;
    // std::cout<<"Accuracy:"<<r[0].score*100<<"%"<<std::endl;
    //who was the Serbian-American inventor
    return 0;
    */
    std::vector<Amber::PageRank::Data> data;
    data.push_back(
     {"https://url.com","Light is the fastest thing. Light is elotromagnetic"}
    );
     data.push_back(
     {"https://tesla.com","Nikola Tesla was a Serbian-American inventor, electrical engineer, mechanical engineer, and futurist best known for his contributions to the design of the modern alternating current electricity supply system. Nikola Tesla was a Serbian-American inventor."}
    );
    Amber::PageRank::PageRank p("who was the Serbian-American inventor",data);
    p.start();
    auto r=p.get_result();
    for(auto& x:r){
        std::cout<<x.url<<":"<<x.score<<":"<<x.description<<std::endl;
    }
}
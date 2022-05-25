#include <ctype.h>
#include <iostream>
#include "Amber/include/tokenizer/tokenizer.hpp"
#include "Amber/include/search/search.hpp"
#include <fstream>
#include <chrono>
#include <sstream>
int main(){
    std::ifstream file("test.txt");
    std::stringstream buf;
    buf << file.rdbuf();
    std::string para=buf.str();
    std::cout<<"Enter a quary:- ";
    std::string para2="";
    std::getline(std::cin,para2) ;//"who was the Serbian-American inventor";
    auto start=std::chrono::high_resolution_clock::now();
    auto tok=Amber::Tokenizer::Lexer(para);
    tok.start();
    auto c=tok.getParagraph();
    auto tok2=Amber::Tokenizer::Lexer(para2);
    tok2.start();
    auto c2=tok2.getParagraph();
    // for(auto& x:c2.sentences){
    //     for(auto& y:x.words){
    //         std::cout<<y<<"\n";
    //     }
    // }
    //measure time
    auto s=Amber::Searcher::Search(c2,c,0.80,10000);
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    s.start();
    auto r=s.get_result();
    std::cout<<"Time taken to show the results: "<<duration.count()<<" milliseconds\n";
    std::cout<<std::endl;
    std::cout<<r[0].sentence.sentance<<std::endl;
    // std::cout<<"Accuracy:"<<r[0].score*100<<"%"<<std::endl;
    //who was the Serbian-American inventor
    return 0;
}
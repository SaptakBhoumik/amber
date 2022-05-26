#include "Amber/include/tokenizer/tokenizer.hpp"
#include <iostream>


int main(){
    std::string para="Light is the fastest thing. Light is an example electromagnetic wave";
    std::cout<<"Text to be tokenized:- "<<para<<"\n";
    auto tokenizer=Amber::Tokenizer::Tokenizer(para);
    tokenizer.start();
    auto tokens=tokenizer.getParagraph();
    std::cout<<"\n\n";
    for(auto& x:tokens.sentences){
        std::cout<<"Sentence:- "<<x.sentence<<"\n";
        for(auto& y:x.words){
            std::cout<<"\t"<<y<<"\n";
        }
    }
}
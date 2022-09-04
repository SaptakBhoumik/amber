#include "Amber/include/tokenizer/tokenizer.hpp"
#include "Amber/include/search/search.hpp"
#include <iostream>

int main(){
    std::string para="Light is the fastest thing in the universe. Light is an example electromagnetic wave";
    std::cout<<"Input paragraph:- "<<para<<"\n";
    auto tokenizer1=Amber::Tokenizer::Tokenizer(para);
    tokenizer1.start();
    auto tokens=tokenizer1.getParagraph();
    std::cout<<"\n\n";
    std::string quary="What is the fastest thing?";
    std::cout<<"Input quary:- "<<quary<<"\n";
    auto tokenizer2=Amber::Tokenizer::Tokenizer(quary);
    tokenizer2.start();
    auto quary_tokens=tokenizer2.getParagraph();
    std::cout<<"\n\n";
    auto search=Amber::Search::Search(quary_tokens,tokens);
    search.start();
    auto res=search.get_result();
    std::cout<<"\nResults:-\n";
    std::cout<<"\tSentence:- "<<res[0].sentence.sentence<<"\n";
    std::cout<<"\tScore:- "<<res[0].score<<"\n";
}
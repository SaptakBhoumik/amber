#include "Amber/include/tokenizer/tokenizer.hpp"
#include "Amber/include/search/search.hpp"
#include "Amber/include/pagerank/pagerank.hpp"
#include <iostream>

int main(){
    std::vector<Amber::PageRank::Data> data;
    data.push_back(
     {"https://en.wikipedia.org/wiki/Light","Light","Light or visible light is electromagnetic radiation within the portion of the electromagnetic spectrum that is perceived by the human eye. Visible light is usually defined as having wavelengths in the range of 400–700 nanometres (nm), corresponding to frequencies of 750-420 terahertz, between the infrared (with longer wavelengths) and the ultraviolet (with shorter wavelengths)."}
    );
     data.push_back(
     {"https://en.wikipedia.org/wiki/Nikola_Tesla","Nikola Tesla", "Nikola Tesla was a Serbian-American inventor, electrical engineer, mechanical engineer, and futurist best known for his contributions to the design of the modern alternating current (AC) electricity supply system"}
    );
    std::cout<<"Seatch data:- \n";
    for(auto& x:data){
        std::cout<<"\tUrl:- "<<x.url<<"\n";
        std::cout<<"\tTitle:- "<<x.title<<"\n";
        std::cout<<"\tContent:- "<<x.content<<"\n";
    }
    std::string quary="who was the Serbian-American inventor";
    std::cout<<"\nQuary:- "<<quary<<"\n\n";
    Amber::PageRank::PageRank p(quary,data);
    p.start();
    auto r=p.get_result();
    std::cout<<"Result:-\n\n";
    std::cout<<"\tUrl:- "<<r[0].url<<"\n";
    std::cout<<"\tDescription:- "<<r[0].description<<"\n";
}

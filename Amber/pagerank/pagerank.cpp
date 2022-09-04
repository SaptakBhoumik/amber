#include "../include/search/search.hpp"
#include "../include/pagerank/pagerank.hpp"
#include <thread>
#include <mutex>
#include <bits/stdc++.h>

namespace Amber{
namespace PageRank{
std::mutex g_mutex;
PageRank::PageRank(std::string quary,std::vector<Data> data,Tokenizer::Language lang,double min_similarity,size_t thread_limit){
    m_lang=lang;
    auto tok_quary=Tokenizer::Tokenizer(quary,m_lang);
    tok_quary.start();
    m_quary=tok_quary.getParagraph();
    if(m_quary.sentences.size()==0){
        return;
    }
    m_thread_limit=thread_limit;
    m_min_similarity=min_similarity;
    create_internal_data(data);
}
std::vector<Result> PageRank::get_result(){
    return m_results;
}

void PageRank::start(){
    if(m_quary.sentences.size()==0||m_data.size()==0){
        return;
    }
    for(auto i=0;i<m_data.size();i++){
        auto search = Search::Search(m_quary,m_data[i].para,m_min_similarity,m_thread_limit);
        search.start();
        auto result=search.get_result();
        if(result.size()==0){
            continue;
        }
        Result r;
        r.description=get_description(result);
        r.score=result[0].score;
        r.url=m_data[i].url;
        m_results.push_back(r);
    }
}

void PageRank::create_internal_data(std::vector<Data>& data){
    std::vector<std::thread> threads;
    for(auto& x:data){
        threads.push_back(std::thread(PageRank::_create_internal_data,this,x,&m_data));
        if(threads.size()>=m_thread_limit){
            for(auto& y:threads){
                y.join();
            }
            threads.clear();
        }
    }
    for(auto& x:threads){
        x.join();
    }
}
void PageRank::_create_internal_data(PageRank* self,Data data,std::vector<InternalData>* collection){
    auto tok=Tokenizer::Tokenizer(data.title+"\n"+data.content,self->m_lang);
    tok.start();
    InternalData temp;
    temp.url=data.url;
    temp.para=tok.getParagraph();
    std::lock_guard<std::mutex> guard(g_mutex);
    collection->push_back(temp);
}
std::string PageRank::get_description(std::vector<Search::Result>& input){
    if(input.size()==0){
        return "";
    }
    auto highest_score=input[0].score;
    std::vector<std::string> candidate;
    for(auto& x:input){
        if(x.score!=highest_score){
            break;
        }
        candidate.push_back(x.sentence.sentence);
    }
    if(candidate.size()==0){
        return "";
    }
    //sort candidate based on length
    std::sort(candidate.begin(),candidate.end(),[](std::string a,std::string b){
        return a.size()<b.size();
    });
    return candidate[0];//The shorter the better
}
}
}
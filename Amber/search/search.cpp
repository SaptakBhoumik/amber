#include "../include/search/search.hpp"
#include <thread>
#include <mutex>
#include <bits/stdc++.h>
namespace Amber{
namespace Search{
std::mutex g_mutex;
double Search::jaccardSimilarity(std::unordered_set<std::string>& quary,std::unordered_set<std::string>& sentence){
    //If the diffrence between the two sets is insiginificant 
    /*
    result=A∩B/A∪B
    */ 
    auto intersection=get_intersection(quary,sentence);
    auto union_set=get_union(quary,sentence);
    return (double)intersection/(double)union_set;
}
double Search::containmentMeasure(std::unordered_set<std::string>& quary,std::unordered_set<std::string>& sentence){
    //If the diffrence between the two sets is insiginificant 
    /*
    result=A∩B/A
    */ 
    auto intersection=get_intersection(quary,sentence);
    return (double)intersection/(double)quary.size();
}
size_t Search::get_union(std::unordered_set<std::string>& A,std::unordered_set<std::string>& B){
    std::unordered_set<std::string> union_set;
    for(auto i:A){
        union_set.insert(i);
    }
    for(auto i:B){
        union_set.insert(i);
    }
    return union_set.size();
}
size_t Search::get_intersection(std::unordered_set<std::string>& A,std::unordered_set<std::string>& B){
    std::unordered_set<std::string> intersection;
    for(auto i:A){
        if(B.contains(i)){
            intersection.insert(i);
        }
    }
    return intersection.size();
}
Search::Search(Tokenizer::Paragraph query, Tokenizer::Paragraph document,double min_similarity,size_t thread_limit){
    for(auto& x:query.sentences){
        for(auto& y:x.words){
            m_query.insert(y);
        }
    }
    m_document=document;
    m_thread_limit=thread_limit;
    m_min_similarity=min_similarity;
}
std::vector<Result> Search::get_result(){
    return m_results;
}
void Search::start(){
    if(m_query.size()==0||m_document.sentences.size()==0){
        return;
    }
    std::vector<std::thread> threads;
    std::vector<Result> res;
    for(auto& x:m_document.sentences){
        threads.push_back(std::thread(_get_res,this,&res,x));
        if(threads.size()>=m_thread_limit){
            for(auto& i:threads){
                i.join();
            }
            threads.clear();
        }
    }
    if(threads.size()>0){

        for(auto& i:threads){
            i.join();
        }
        threads.clear();
    }
    if(res.size()>0){
        std::sort(res.begin(),res.end(),[](Result& a,Result& b){
            return a.score>b.score;
        });
    }
    m_results=res;
}
void Search::_get_res(Search* self,std::vector<Result>* res,Tokenizer::Sentence sentence){
    double simmilarity=0;
    double size_difference=0;
    {
        double sentence_len=(double)sentence.words.size();
        double quary_len=(double)self->m_query.size();
        size_difference=(sentence_len-quary_len)/quary_len;
    }
    if(size_difference<0){
        size_difference*=-1;
    }
    if(size_difference<=0.2){
        simmilarity=self->jaccardSimilarity(self->m_query,sentence.words);
    }
    else{
        simmilarity=self->containmentMeasure(self->m_query,sentence.words);
    }
    if(sentence.emotion==Question){
        //We prefer the answer more
        simmilarity*=0.9;
    }
    if(simmilarity>=self->m_min_similarity){
        Result result;
        result.sentence=sentence;
        result.score=simmilarity;
        std::lock_guard<std::mutex> guard(g_mutex);
        res->push_back(result);
    }
}
}
}
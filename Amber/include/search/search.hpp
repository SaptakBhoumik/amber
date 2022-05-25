#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__
#include "../tokenizer/tokenizer.hpp"
namespace Amber{
namespace Searcher{
    struct Result{
        Tokenizer::Sentence sentence;
        double score;
    };
    class Search{
        public:
        Search(Tokenizer::Paragraph query, Tokenizer::Paragraph document,double min_similarity = 0.5,size_t thread_limit=1000);
        std::vector<Result> get_result();
        void start();
        private:

        std::unordered_set<std::string> m_query;
        Tokenizer::Paragraph m_document;
        size_t m_thread_limit;
        std::vector<Result> m_results;
        double m_min_similarity;

        double jaccardSimilarity(std::unordered_set<std::string>&,std::unordered_set<std::string>&);
        double containmentMeasure(std::unordered_set<std::string>& quary,std::unordered_set<std::string>& sentence);
        std::unordered_set<std::string> get_union(std::unordered_set<std::string>&,std::unordered_set<std::string>&);
        std::unordered_set<std::string> get_intersection(std::unordered_set<std::string>&,std::unordered_set<std::string>&);
        static void _get_res(Search*,std::vector<Result>*,Tokenizer::Sentence);
    };
}
}
#endif
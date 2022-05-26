#ifndef __PAGERANK_HPP__
#define __PAGERANK_HPP__
#include <vector>
#include <string>
#include "../tokenizer/tokenizer.hpp"

namespace Amber{
namespace PageRank{
    struct Data{
        std::string url;
        std::string title;
        std::string content;
    };
    struct Result{
        std::string url;
        std::string description;
        double score;
    };
    class PageRank{
        public:
        PageRank(std::string quary,std::vector<Data>,Tokenizer::Language lang=Tokenizer::Language::English,double min_similarity = 0.5,size_t thread_limit=1000);
        std::vector<Result> get_result();
        void start();
        private:
        struct InternalData{
            std::string url;
            Tokenizer::Paragraph para;
        };
        Tokenizer::Paragraph m_quary;
        std::vector<InternalData> m_data;
        size_t m_thread_limit;
        std::vector<Result> m_results;
        double m_min_similarity;
        Tokenizer::Language m_lang;
        std::string get_description(std::vector<Search::Result>&);
        void create_internal_data(std::vector<Data>&);
        static void _create_internal_data(PageRank*,Data,std::vector<InternalData>*);
    };
}
}
#endif
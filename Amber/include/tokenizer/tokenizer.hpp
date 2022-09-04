#ifndef __TOKENIZER__HPP
#define __TOKENIZER__HPP
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "special_char.hpp"
namespace Amber{
namespace Tokenizer{
    enum Language{
        English,
        Bengali
    };
    #include "emotion.hpp"
    struct Sentence{
        std::string sentence;
        std::unordered_set<std::string> words;
        Emotion emotion=None;
    };
    struct Paragraph{
        std::vector<Sentence> sentences;
    };
    class Tokenizer{
        public:
            Tokenizer(std::string,Language lang=English);
            Paragraph getParagraph();
            void start();
        private:
            std::string m_text;
            Language m_lang;
            Paragraph m_paragraphs;
            std::unordered_map<Language, std::unordered_set<std::string>> m_stopword_map={
                {English, StopWord::english_stop_words},
                {Bengali, StopWord::bengali_stop_words}
            };
            std::unordered_map<Language, std::unordered_map<wchar_t,Emotion>> m_sentence_stopper_map={
                {English, SentenceStopper::english_sentence_stopper},
                {Bengali, SentenceStopper::bengali_sentence_stopper}
            };
            std::unordered_map<Language, std::unordered_set<wchar_t>> m_punctuation_map={
                {English, Punctuation::english_punctuation},
                {Bengali, Punctuation::english_punctuation}
            };

            void tokenize();
            wchar_t peek(size_t);
    };
    }
}
#endif
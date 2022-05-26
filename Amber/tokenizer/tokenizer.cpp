#include "../include/tokenizer/tokenizer.hpp"
#include <ctype.h>
#include <iostream>

#define add_unknown() if(temp_word.size() > 0){\
                        if(!stop_word.contains(temp_word)){\
                            temp_sentence_words.insert(temp_word);\
                        }\
                        temp_word.clear();\
                    }

namespace Amber{
namespace Tokenizer{
    Tokenizer::Tokenizer(std::string text,Language lang){
        m_text = text;
        m_lang = lang;
    }
    void Tokenizer::start(){
        if(m_text.size() > 0){
            if(!m_sentence_stopper_map[m_lang].contains(m_text.back())){
                m_text.push_back('\n');
            }
            tokenize();
        }
    }
    Paragraph Tokenizer::getParagraph(){
        return m_paragraphs;
    }
    void Tokenizer::tokenize(){
        std::string temp_sentence = "";//the currrent sentence
        std::string temp_word = "";//the current word
        std::unordered_set<std::string> temp_sentence_words;
        auto puntuation = m_punctuation_map[m_lang];
        auto stop_sentence=m_sentence_stopper_map[m_lang];
        auto stop_word = m_stopword_map[m_lang];
        for(size_t i=0;i<m_text.size();++i){
            wchar_t curr_item=tolower(m_text[i]);
            if(curr_item==' '||curr_item=='\t'){
                add_unknown();
                temp_sentence+= m_text[i];
            }
            else if(puntuation.contains(curr_item)){
                add_unknown();
                temp_sentence+= m_text[i];
            }
            else if(curr_item=='.' && m_lang==English && 
                    (stop_sentence.contains(peek(i+2))||
                    (peek(i-2)=='.'&&stop_sentence.contains(peek(i+2))))){
                //It's an abbreviation
                temp_sentence+= m_text[i];
                temp_word +=curr_item;
            }
            else if(stop_sentence.contains(curr_item)){
                add_unknown();
                auto emotion=stop_sentence[curr_item];
                Sentence temp;
                if(temp_sentence_words.size() > 0){
                    temp.sentence = temp_sentence;
                    temp.words = temp_sentence_words;
                    temp.emotion = emotion;
                    m_paragraphs.sentences.push_back(temp);
                    temp_sentence.clear();
                    temp_sentence_words.clear();
                }
            }
            else{
                temp_sentence+= m_text[i];
                temp_word +=curr_item;
            }
        }
        add_unknown();
        Emotion emotion=None;
        if(stop_sentence.contains(m_text.back())){
            emotion=stop_sentence[m_text.back()];
        }
        Sentence temp;
        if(temp_sentence_words.size() > 0){
            temp.sentence = temp_sentence;
            temp.words = temp_sentence_words;
            temp.emotion = emotion;
            m_paragraphs.sentences.push_back(temp);
            temp_sentence.clear();
            temp_sentence_words.clear();
        }
    }
    wchar_t Tokenizer::peek(size_t i){
        if(i<=m_text.size()-1){
            return m_text[i];
        }
        return '\0';
    }
}
}
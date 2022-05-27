# Thank you for your interest in Amber

## Add a new language

-   To add a new language first open "Amber/tokenizer/special_char.cpp"
    
        Then add the new language first add the a stop word of the language. Stop words are the most commonly used words in that language which are often repeated. Note that the stop words should be in lower case and should be defined within namespace Amber::StopWord.A simple example is given below.
        
        ```cpp
        std::unordered_set<std::string> languagename_stop_words= {"word1", "word2", "word3"};
        ```

        Then add the symbols that are used to seperate sentence. Make sure that they are defined within  namespace Amber::SentenceStopper. An example is given below.

        ```cpp
        std::unordered_map<wchar_t,Emotion> languagename_sentence_stopper={{'.',TypeOfSentence},{'\n',None},{'\r',None}};
        ```

        Now finally add the punctuation of language. Make sure that they are defined within namespace Amber::Punctuation and dont redefine punctuation that are already defined in ``languagename_sentence_stopper``. An example is given below.

        ```cpp
        std::unordered_set<wchar_t> languagename_punctuation={',',':',';','\'}
        ```

- Now open "Amber/include/tokenizer/special_char.hpp" and define the things that you have already defined in "Amber/tokenizer/special_char.cpp".

- Now open "Amber/include/tokenizer/tokenizer.hpp" and add the language that you have created to the ``Amber::Tokenizer::Language`` enum. Then add the variables you have defined to the m_stopword_map, m_sentence_stopper_map and m_punctuation_map variable of the tokeizer class.



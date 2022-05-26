# Amber Documentation

## Tokenizer

The tokenizer breaks down the paragraph into sentences and splits each sentence into words.It will also remove punctuation, special characters and commonly used words for better predictability.

### Api overview

- Important enum definitions
    - Amber::Tokenizer::Emotion
        Describes whether the sentence is a question,exclamation, or none
        ```cpp
        enum Emotion{
            Question=0,
            Exclamation=1,
            None=2,
        };
        ```
    - Amber::Tokenizer::Language
        Describes the language of the sentence
        ```cpp
        enum Language{
            English,
            Bengali
        };
        ```

- Important struct definitions
    - Amber::Tokenizer::Sentence
        Individual sentence of the paragraph
        ```cpp
        struct Sentence{
            std::string sentence; // The sentence
            std::unordered_set<std::string> words; // The words in the sentence
            Emotion emotion=None;// The emotion/type of the sentence
        };
        ```
    - Amber::Tokenizer::Paragraph
        The paragraph which is a collection of sentences
        ```cpp
        struct Paragraph{
            std::vector<Sentence> sentences;
        };
        ```

- Tha main tokenizer class
    - Amber::Tokenizer::Tokenizer(std::string paragraph,Amber::Tokenizer::Language lang=English)
        Initialize the main Tokenizer class
        ```cpp
        auto tokenizer = Amber::Tokenizer::Tokenizer(paragraph_to_be_tokenized,language_of_the_paragraph);
        ```
    - void Amber::Tokenizer::Tokenizer::start()
        Starts the tokenizer
        ```cpp
        tokenizer.start();
        ```
    - Amber::Tokenizer::Paragraph Amber::Tokenizer::Tokenizer::getParagraph()
        Returns the tokenized paragraph
        ```cpp
        auto tokenized_paragraph = tokenizer.getParagraph();
        ```


## Search

The search function will try to find the best answer for your quary from the tokenized paragraph. And arrange the sentences in the decending order of their relevance.

### Api overview

- Important struct definitions
    - Amber::Search::Result
        The sentence and its relevance
        ```cpp
        struct Result{
            Amber::Tokenizer::Sentence sentence;
            double score;
        };
        ```

- Main search class
    - Amber::Search::Search(Amber::Tokenizer::Paragraph query, Amber::Tokenizer::Paragraph document,double min_similarity = 0.5,size_t thread_limit=1000)
        Initialize the main Search class
        ```cpp
        auto search = Amber::Search::Search(tokenized_query,tokenized_document,min_similarity,thread_limit);
        ```

    - void Amber::Search::Search::start()
        Starts the search
        ```cpp
        search.start();
        ```
    
    - std::vector<Amber::Search::Result> Amber::Search::Search::get_result()
        Returns the search result
        ```cpp
        auto search_result = search.get_result();
        ```


## Page Ranker
    
Rank the pages based on their relevance. It will also return a short description of the page based on the quary.

### Api overview

- Important struct definitions
    - Amber::PageRank::Data
        The input data that contains the url,title and content of the webpage
        ```cpp
        struct Data{
            std::string url;
            std::string title;
            std::string content;
        };
        ```

    - Amber::PageRank::Result
        The result of the page ranker
        ```cpp
        struct Result{
            std::string url;// The url of the webpage
            std::string description; //A short description of the page based on the quary
            double score;// The relevance of the page
        };
        ```

- Main PageRank class
    - Amber::PageRank::PageRank(std::string quary,std::vector<Data>,Tokenizer::Language    lang=Tokenizer::Language::English,double min_similarity = 0.5,size_t thread_limit=1000);
        Initialize the main PageRank class
        ```cpp
        auto page_ranker = Amber::PageRank::PageRank(quary,data,min_similarity,thread_limit);
        ```

    - void Amber::PageRank::PageRank::start()
        Starts the PageRank
        ```cpp
        page_ranker.start();
        ```
    
    - std::vector<Amber::PageRank::Result> Amber::PageRank::PageRank::get_result()
        Returns the result of the PageRank
        ```cpp
        auto page_ranker_result = page_ranker.get_result();
        ```
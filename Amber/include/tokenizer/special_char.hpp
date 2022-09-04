#include <unordered_set>
#include <unordered_map>
#include <string>
#include "emotion.hpp"
namespace Amber{
namespace StopWord{
extern std::unordered_set<std::string> english_stop_words;
extern std::unordered_set<std::string>  bengali_stop_words;
}
namespace SentenceStopper{
extern std::unordered_map<wchar_t,Emotion> english_sentence_stopper;
extern std::unordered_map<wchar_t,Emotion> bengali_sentence_stopper;
}
namespace Punctuation{
extern std::unordered_set<wchar_t> english_punctuation;// Bengali Punctuation is same as english
}
}
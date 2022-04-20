#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include "gumbo.h"
struct Data{
    std::string content;
    std::unordered_set<std::string> url;
};
__attribute__((always_inline)) inline std::string full_url(std::string original,std::string part){
    if(part.size()>8){
        if(part.substr(0,8)=="https://"){}
    }
    return part;
}
static Data get_url_text(GumboNode *node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return {
          std::string(node->v.text.text),
          {""}
        };
    }
    else if (node->v.element.tag == GUMBO_TAG_A){
      return {
        "",
        {gumbo_get_attribute(&node->v.element.attributes, "href")->value}
      };
    }
    else if (node->type == GUMBO_NODE_ELEMENT &&
              node->v.element.tag != GUMBO_TAG_SCRIPT &&
              node->v.element.tag != GUMBO_TAG_STYLE) {
        Data contents;
        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
          const Data text = get_url_text((GumboNode *)children->data[i]);
          if(text.url.size()==0&&text.content.empty()){
            contents.content.append(" ");
          }
          else if(text.url.size()>0){
            for(auto& i:text.url){
              if(!i.empty()){
                contents.url.insert(i);
              }
            }
          }
          contents.content.append(text.content);
          // if (i != 0 && !text.empty()) {
          //   contents.append(" ");
          // }
          // contents.append(text);
        }
      return contents;
    } 
    else {
      return Data{};
    }
}

int main(int argc, char **argv) {
    const char *filename = "test.html";
    
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in) {
      std::cout << "File " << filename << " not found!\n";
      exit(EXIT_FAILURE);
    }

    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();

    GumboOutput *output = gumbo_parse(contents.c_str());
    auto x=get_url_text(output->root);
    std::cout << x.content << std::endl;
    for(auto& i:x.url){
      std::cout<<i<<std::endl;
    }
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}
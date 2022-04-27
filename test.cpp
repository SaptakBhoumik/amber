#include "gumbo.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_set>
struct Data {
  std::string content = "";
  std::string title = "";
  std::unordered_set<std::string> url = {};
};
std::string full_url(std::string original,std::string part) {
    if(part.find('#') != std::string::npos || 
        part.find('?') != std::string::npos ||
        part.find('@') != std::string::npos ||
        part=="/"||part==original){
        return "";
    }
    else if(part[0]=='/'){
        return original+part;
    }
    else if(part.find("https://") != std::string::npos || 
        part.find("http://") != std::string::npos){
        return part;
    }
    return part;
}
static Data get_url_text(GumboNode *node,std::string original_url) {
    if (node->type == GUMBO_NODE_TEXT) {
      return {.content = std::string(node->v.text.text), .url = {""}};
    } else if (node->v.element.tag == GUMBO_TAG_A) {
        auto x=gumbo_get_attribute(&node->v.element.attributes, "href");
        std::string url;
        if(x==NULL){
            url="";
        }
        else{
            url=full_url(original_url,x->value);
        }
        return {
            .content = "",
            .url = {url}
            };
    } else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE) {
      Data contents;
      GumboVector *children = &node->v.element.children;
      for (unsigned int i = 0; i < children->length; ++i) {
        const Data text = get_url_text((GumboNode *)children->data[i],original_url);
        if (text.url.size() > 0) {
          for (auto &i : text.url) {
            if (!i.empty()) {
              contents.url.insert(i);
            }
          }
        }
        if (node->v.element.tag == GUMBO_TAG_TITLE) {
          contents.title.append(text.content);
        } else {
          contents.title.append(text.title);
          contents.content.append(text.content);
        }
      }
      return contents;
    } else {
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
    auto x = get_url_text(output->root,"https://github.com");
    std::cout << x.content << std::endl;
    for (auto &i : x.url) {
      std::cout << "URL = " << i << std::endl;
    }
    std::cout << "Title = " << x.title << std::endl;
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}
#include <iostream>
#include <string>
#include <curl/curl.h>
#define USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWfdgebKit/537.36 (KHTML, like Gecko) Chrome/99 Safari/537.36"
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  std::string* str=(std::string*)userp;
  str->reserve(realsize);
  auto c_str=(char*)contents;
  for(size_t i=0;i<realsize;i++){
    str->push_back(c_str[i]);
  }
  return realsize;
}
int main(int argc, char *argv[]) {
  if( argc != 2 ) {
    printf("usage: try './curl [url]' to make a get request.\n");
    return 1;
  }

  CURL *curl_handle;
  CURLcode res;

  std::string chunk;

  curl_handle = curl_easy_init();
  if(curl_handle) {
    curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT);

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
      fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
    } else {
      // printf("Size: %lu\n", (unsigned long)chunk.size);
      // printf("Data: %s\n", chunk.memory);
      // std::cout<<chunk;
    }
    curl_easy_cleanup(curl_handle);

  }
  else{
    std::cout<<"Error: Couldn't create a curl instance";
    exit(1);
  }
  return 0;
}

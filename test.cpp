#include "include/crawler/html.hpp"
#include "include/crawler/crawler.hpp"
#include "include/database/database.hpp"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <filesystem>
#include <sys/resource.h>
using namespace Amber;
int main(){
    DataDB ref=DataDB("/home/saptak/Desktop/projects/amber/dataset/data/db1.db");
    std::cout<<&ref<<std::endl;
}
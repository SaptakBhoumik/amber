#include <vector>
#include <iostream>

int main(){
    std::vector<int> i;
    i.reserve(12);
    std::cout<<i[11]<<std::endl;
}
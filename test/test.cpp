#include <iostream>
#include "../binchunk/reader/BinaryReader.h"

int main () {
    using namespace std;
    BinaryReader read("/Users/fss/CLionProjects/Luacpp/test/luac.out");
    read.checkReader();
    return 0 ;
}
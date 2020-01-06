#include <iostream>
#include "../binchunk/reader/BinaryReader.h"

int main () {
    using namespace std;
    BinaryReader reader("/Users/fss/CLionProjects/Luacpp/test/luac.out");
    reader.checkReader();
    reader.readByte();
    reader.readProto("");
}

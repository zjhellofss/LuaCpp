//
// Created by fushenshen on 2020/1/5.
//

#ifndef LUACPP_BINARYREADER_H
#define LUACPP_BINARYREADER_H

#include <string>
#include <cassert>
#include <vector>

#include "../BinaryChunk.h"
#include "../../exception/HeaderException.h"


struct BinaryReader {
public:
    ~BinaryReader () {
        if (!file) {
            fclose(file);
            delete[]bytes;
            file = nullptr;
        }
    }

    byte readByte ();

    std::vector<byte> readBytes (const int &len);

    uint32 readUint32 ();

    uint32 readUint32Code ();

    uint64_t readUint64 ();

    int64_t readLuaInteger ();

    double readLuaNumber ();

    std::string readString ();

    void checkReader ();

    std::shared_ptr<ProtoType> readProto (const std::string &parentSource);

    std::vector<uint32> readCode ();

    Interface* readConstant ();

    std::vector<Interface*> readConstants ();

    std::vector<Upvalue> readUpValues ();

    std::vector<uint32> readLineInfo ();

    std::vector<LocVal> readLocVals ();

    std::vector<std::string> readUpValueNames ();

    std::vector<std::shared_ptr<ProtoType>> readProtos (const std::string &parentSource);

public:
    explicit BinaryReader (const std::string &filePath) : filePath(filePath),
                                                          pos(0), file(fopen(filePath.data(), "r")) {
        this->file = fopen(filePath.data(), "r");
        fseek(this->file, 0, SEEK_END);
        this->fileSize = static_cast<uint32>(ftell(this->file));
        fseek(this->file, 0, SEEK_SET);
        bytes = new byte[fileSize];
        fread(bytes, sizeof(byte), this->fileSize,
              this->file);
    }

private:
    std::string filePath;
    uint32 fileSize;
    FILE *file;
    byte *bytes;
    int pos;
private:

};

#endif //LUACPP_BINARYREADER_H

//
// Created by fushenshen on 2020/1/6.
//

#ifndef LUACPP_BINARYPRINTER_H
#define LUACPP_BINARYPRINTER_H

#include <string>
#include "../BinaryChunk.h"

void printBinary (const std::string &path);

std::shared_ptr<ProtoType> undump (const std::string &path);


#endif //LUACPP_BINARYPRINTER_H

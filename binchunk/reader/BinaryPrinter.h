//
// Created by fushenshen on 2020/1/6.
//

#ifndef LUACPP_BINARYPRINTER_H
#define LUACPP_BINARYPRINTER_H

#include <string>
#include "../BinaryChunk.h"

void printHeader (ProtoType *f) ;

std::string constantToString (Interface *inst);

std::string upValName (ProtoType *f, int idx);

void printDetial (ProtoType *f);

std::shared_ptr<ProtoType> undump (const std::string &path);

void printBinary (const std::string &path);

void printOperands (uint32_t instruction);

void printCode (ProtoType *f);

void list (ProtoType *f);


#endif //LUACPP_BINARYPRINTER_H

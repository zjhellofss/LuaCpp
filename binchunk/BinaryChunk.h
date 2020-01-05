#include <utility>

//
// Created by fushenshen on 2020/1/5.
//

#ifndef LUACPP_BINARYCHUNK_H
#define LUACPP_BINARYCHUNK_H

#include <string>
#include <vector>

using byte = unsigned char;
using uint32 = unsigned int;
//chunk文件头部的签名常量
const std::string LUA_SIGNATURE("\x1bLua");
//...的版本号
byte LUAC_VERSION = 0x53;
//...的格式号
byte LUAC_FORMAT = 0x00;

const std::string LUAC_DATA("\x19\x93\r\n\x1a\n");

byte CINT_SIZE = 4;
byte INSTRUCTION_SIZE = 4;
byte CSIZET_SIZE = 8;
byte LUA_INTEGER_SIZE = 8;
byte LUA_NUMBER_SIZE = 8;

uint32 LUAC_INT = 0x5678;
const double LUAC_NUM = 370.5;

//chunk的头部
struct Header {
    byte signature[4];
    byte version;
    byte format;
    byte luacData[6];

    byte cintSize;
    byte sizetSize;
    byte instructionSize;
    byte luaIntegerSize;
    byte luaNumberSize;

    int64_t luacInt;
    double luacNum;
};


byte TAG_NIL = 0x00;
byte TAG_BOOLEAN = 0x01;
byte TAG_NUMBER = 0x03;
byte TAG_INTEGER = 0x13;
byte TAG_SHORT_STR = 0x04;
byte TAG_LONG_STR = 0x14;


struct Upvalue {
    byte instack;
    byte idx;

    Upvalue (byte instack, byte idx) : instack(instack), idx(idx) {}
};

struct LocVal {
    std::string varName;
    uint32 startPC;
    uint32 endPC;

    LocVal (std::string varName, uint32 startPC, uint32 endPC) : varName(std::move(varName)), startPC(startPC),
                                                                 endPC(endPC) {}
};

union val {
    std::string str;
    uint64_t val;
    double number;
};

struct interface {
public:
    int type;
public:
    bool boolean;
    int byteVal;
    double number;
    int64_t integer;
    std::string valStr;
};

struct ProtoType {
    std::string source;
    uint32 lineDefined;
    uint32 lastLineDefined;
    byte numParams;
    byte isVarag;
    //最大寄存器数量
    byte maxStackSize;
    std::vector<uint32> code;
    std::vector<interface> constants;
    std::vector<Upvalue> upvalues;
    std::vector<std::shared_ptr<ProtoType>> protos;
    //debug information
    std::vector<uint32> lineInfo;
    std::vector<LocVal> locVar;
    std::vector<std::string> upvalueNames;

    ProtoType (const std::string &source, uint32 lineDefined, uint32 lastLineDefined, byte numParams, byte isVarag,
               byte maxStackSize, const std::vector<uint32> &code, const std::vector<interface> &constants,
               const std::vector<Upvalue> &upvalues, const std::vector<std::shared_ptr<ProtoType>> &protos,
               const std::vector<uint32> &lineInfo, const std::vector<LocVal> &locVar,
               const std::vector<std::string> &upvalueNames) : source(source), lineDefined(lineDefined),
                                                               lastLineDefined(lastLineDefined), numParams(numParams),
                                                               isVarag(isVarag), maxStackSize(maxStackSize), code(code),
                                                               constants(constants), upvalues(upvalues), protos(protos),
                                                               lineInfo(lineInfo), locVar(locVar),
                                                               upvalueNames(upvalueNames) {}
};

struct BinaryChunk {
    Header header;
    byte sizeUpvalues;
};


#endif //LUACPP_BINARYCHUNK_H

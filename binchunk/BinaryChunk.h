#include <utility>

#ifndef LUACPP_BINARYCHUNK_H
#define LUACPP_BINARYCHUNK_H

#include <string>
#include <vector>

using byte = unsigned char;
using uint32 = unsigned int;

#define  TAG_NIL  0x00
#define TAG_BOOLEAN  0x01
#define TAG_NUMBER  0x03
#define TAG_INTEGER  0x13
#define TAG_SHORT_STR  0x04
#define TAG_LONG_STR  0x14

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

    ProtoType (std::string source, uint32 lineDefined, uint32 lastLineDefined, byte numParams, byte isVarag,
               byte maxStackSize, const std::vector<uint32> &code, const std::vector<interface> &constants,
               std::vector<Upvalue> upvalues, const std::vector<std::shared_ptr<ProtoType>> &protos,
               std::vector<uint32> lineInfo, const std::vector<LocVal> &locVar,
               std::vector<std::string> upvalueNames) : source(std::move(source)), lineDefined(lineDefined),
                                                        lastLineDefined(lastLineDefined), numParams(numParams),
                                                        isVarag(isVarag), maxStackSize(maxStackSize), code(code),
                                                        constants(constants), upvalues(std::move(upvalues)),
                                                        protos(protos),
                                                        lineInfo(std::move(lineInfo)), locVar(locVar),
                                                        upvalueNames(std::move(upvalueNames)) {}
};

struct BinaryChunk {
    Header header;
    byte sizeUpvalues;
    std::shared_ptr<ProtoType> proto;
};


#endif //LUACPP_BINARYCHUNK_H

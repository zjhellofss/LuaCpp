#include <utility>

#ifndef LUACPP_BINARYCHUNK_H
#define LUACPP_BINARYCHUNK_H

#include <string>
#include <vector>

using byte = unsigned char;

#define  TAG_NIL  0x00
#define TAG_BOOLEAN  0x01
#define TAG_NUMBER  0x03
#define TAG_INTEGER  0x13
#define TAG_SHORT_STR  0x04
#define TAG_LONG_STR  0x14
#define CINT_SIZE  4
#define INSTRUCTION_SIZE 4
#define CSIZET_SIZE  8
#define LUA_INTEGER_SIZE  8
#define LUA_NUMBER_SIZE 8

//chunk文件头部的签名常量
#define LUA_SIGNATURE "\x1bLua"
//...的版本号
#define LUAC_VERSION  0x53
//...的格式号
#define LUAC_FORMAT  0x00
#define LUAC_DATA "\x19\x93\r\n\x1a\n"


#define LUAC_INT 0x5678
#define LUAC_NUM  370.5


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
    uint32_t startPC;
    uint32_t endPC;

    LocVal (std::string varName, uint32_t startPC, uint32_t endPC) : varName(std::move(varName)), startPC(startPC),
                                                                 endPC(endPC) {}
};

struct Interface {
    int type;
    void *val;

    virtual ~Interface () {
        if (type == TAG_BOOLEAN) {
            delete (int *) val;
        } else if (type == TAG_SHORT_STR || type == TAG_LONG_STR) {
            delete (std::string *) val;
        } else if (type == TAG_INTEGER) {
            delete (int *) val;
        } else if (type == TAG_NUMBER) {
            delete (double *) val;
        } else {

        }
    }
};

struct ProtoType {
    std::string source;
    uint32_t lineDefined;
    uint32_t lastLineDefined;
    byte numParams;
    byte isVarag;
    //最大寄存器数量
    byte maxStackSize;
    std::vector<uint32_t> code;
    std::vector<Interface *> constants;
    std::vector<Upvalue> upvalues;
    std::vector<std::shared_ptr<ProtoType>> protos;
    //debug information
    std::vector<uint32_t> lineInfo;
    std::vector<LocVal> locVar;
    std::vector<std::string> upvalueNames;

    ProtoType (std::string source, uint32_t lineDefined, uint32_t lastLineDefined, byte numParams, byte isVarag,
               byte maxStackSize, const std::vector<uint32_t> &code, const std::vector<Interface *> &constants,
               std::vector<Upvalue> upvalues, const std::vector<std::shared_ptr<ProtoType>> &protos,
               std::vector<uint32_t> lineInfo, const std::vector<LocVal> &locVar,
               std::vector<std::string> upvalueNames) : source(std::move(source)), lineDefined(lineDefined),
                                                        lastLineDefined(lastLineDefined), numParams(numParams),
                                                        isVarag(isVarag), maxStackSize(maxStackSize), code(code),
                                                        constants(constants), upvalues(std::move(upvalues)),
                                                        protos(protos),
                                                        lineInfo(std::move(lineInfo)), locVar(locVar),

                                                        upvalueNames(std::move(upvalueNames)) {}

    virtual ~ProtoType () {
        for (int i = 0; i < this->constants.size(); ++i) {
            Interface *f = this->constants[i];
            delete f;
        }
    }
};

struct BinaryChunk {
    Header header;
    byte sizeUpvalues;
    std::shared_ptr<ProtoType> proto;
};


#endif //LUACPP_BINARYCHUNK_H

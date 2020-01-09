//
// Created by fushenshen on 2020/1/6.
//

#ifndef LUACPP_OPCODE_H
#define LUACPP_OPCODE_H

#include "BinaryChunk.h"
#include "../api/LuaVm.h"

const int MAXARG_Bx = (1 << 18) - 1;       // 262143
const int MAXARG_sBx = (MAXARG_Bx >> 1);
enum Opcode {
    OP_MOVE = 0,
    OP_LOADK,
    OP_LOADKX,
    OP_LOADBOOL,
    OP_LOADNIL,
    OP_GETUPVAL,
    OP_GETTABUP,
    OP_GETTABLE,
    OP_SETTABUP,
    OP_SETUPVAL,
    OP_SETTABLE,
    OP_NEWTABLE,
    OP_SELF,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_MOD,
    OP_POW,
    OP_DIV,
    OP_IDIV,
    OP_BAND,
    OP_BOR,
    OP_BXOR,
    OP_SHL,
    OP_SHR,
    OP_UNM,
    OP_BNOT,
    OP_NOT,
    OP_LEN,
    OP_CONCAT,
    OP_JMP,
    OP_EQ,
    OP_LT,
    OP_LE,
    OP_TEST,
    OP_TESTSET,
    OP_CALL,
    OP_TAILCALL,
    OP_RETURN,
    OP_FORLOOP,
    OP_FORPREP,
    OP_TFORCALL,
    OP_TFORLOOP,
    OP_SETLIST,
    OP_CLOSURE,
    OP_VARARG,
    OP_EXTRAARG,
};

enum OpArgMask {
    OpArgN,         //操作数不表示任何信息
    OpArgU,
    OpArgR,        //操作数作为寄存器索引或者跳转的偏移
    OpArgK        // 操作数作为寄存器或者常量表索引
};


struct OpcodeStruct {
private:
    byte testFlag;  //jump操作之前的
    byte setAFlag;  // 设置寄存器A
    byte argBMode;  // 设置寄存器B
    byte argCMode;  // 设置寄存器C
    byte opMode; // opcode
    std::string name;//操作的名称
public:
    void (LuaVm::*action) (uint32_t);

public:

    byte getTestFlag () const {
        return testFlag;
    }

    void setTestFlag (byte testFlag) {
        OpcodeStruct::testFlag = testFlag;
    }

    byte getSetAFlag () const {
        return setAFlag;
    }

    void setSetAFlag (byte setAFlag) {
        OpcodeStruct::setAFlag = setAFlag;
    }

    byte getArgBMode () const {
        return argBMode;
    }

    void setArgBMode (byte argBMode) {
        OpcodeStruct::argBMode = argBMode;
    }

    byte getArgCMode () const {
        return argCMode;
    }

    void setArgCMode (byte argCMode) {
        OpcodeStruct::argCMode = argCMode;
    }

    byte getOpMode () const {
        return opMode;
    }

    void setOpMode (byte opMode) {
        OpcodeStruct::opMode = opMode;
    }

    const std::string &getName () const {
        return name;
    }

    void setName (const std::string &name) {
        OpcodeStruct::name = name;
    }

public:
    OpcodeStruct (byte testFlag, byte setAFlag, byte argBMode, byte argCMode, byte opMode, const std::string &name,
                  void (LuaVm::*action) (uint32_t)) : testFlag(testFlag), setAFlag(setAFlag), argBMode(argBMode),
                                                      argCMode(argCMode), opMode(opMode), name(name), action(action) {}
};


//指令的格式
enum Opmode {
    IABC,
    IABx,
    IAsBx,
    IAx,
};


uint32_t getOpcode (uint32_t instruction);

//获取ABCmode a b c
std::tuple<int, int, int> ABC (uint32_t instruction);

//获取ABXmode a bx
std::tuple<int, int> ABX (uint32_t instruction);

//获取AXmode中的参数
int AX (uint32_t instruction);

std::tuple<int, int> AsBx (uint32_t instruction);

std::string opName (uint32_t instruction);

byte opMode (uint32_t instruction);

byte bMode (uint32_t instruction);

byte cMode (uint32_t instruction);

void execute (uint32_t instruction, LuaVm *luaVm);

#endif //LUACPP_OPCODE_H

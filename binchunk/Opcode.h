//
// Created by fushenshen on 2020/1/6.
//

#ifndef LUACPP_OPCODE_H
#define LUACPP_OPCODE_H

#include "BinaryChunk.h"


const int MAXARG_Bx = (1 << 18) - 1;       // 262143
const int MAXARG_sBx = (MAXARG_Bx >> 1);
enum Opcode {
    OP_MOVE,
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
    byte testFlag;  //jump操作之前的
    byte setAFlag;  // 设置寄存器A
    byte argBMode;  // 设置寄存器B
    byte argCMode;  // 设置寄存器C
    byte opMode; // opcode
    std::string name;//操作的名称

    OpcodeStruct (byte testFlag, byte setAFlag, byte argBMode, byte argCMode, byte opMode,
                  std::string name) : testFlag(testFlag), setAFlag(setAFlag), argBMode(argBMode),
                                      argCMode(argCMode), opMode(opMode), name(std::move(name)) {}
};


//指令的格式
enum Opmode {
    IABC,
    IABx,
    IAsBx,
    IAx,
};



uint32 getOpcode (uint32 instruction);

//获取ABCmode a b c
std::tuple<int, int, int> ABC (uint32 instruction);

//获取ABXmode a bx
std::tuple<int, int> ABX (uint32 instruction);

//获取AXmode中的参数
int AX (uint32 instruction) ;

std::tuple<int, int> AsBx (uint32 instruction);

std::string opName (uint32 instruction);

byte opMode (uint32 instruction);

byte bMode (uint32 instruction);

byte cMode (uint32 instruction);


#endif //LUACPP_OPCODE_H
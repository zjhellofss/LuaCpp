//
// Created by fushenshen on 2020/1/8.
//

#ifndef LUACPP_LUAVM_H
#define LUACPP_LUAVM_H


#include "../luaState/LuaState.h"


class LuaVm {
private:
    LuaState luaState;
public:
    LuaVm (ProtoType *protoType);

    uint32_t fetch ();

    void addPc (int n);

    void getConst (int idx);

    void getRk (int rk);

    int Pc ();

    ProtoType *getProtoType ();

    void move (uint32_t instruction);

    void jmp (uint32_t instruction);

    void loadNil (uint32_t instruction);

    void loadBool (uint32_t instruction);

    void loadK (uint32_t instruction);

    void loadKx (uint32_t instruction);

    void _binaryArith (uint32_t instruction, LuaValueOperator op);

    void _unaryArith (uint32_t instruction, LuaValueOperator op);

    void _len (uint32_t instruction);

    void concat (uint32_t instruction);

    void _not (uint32_t instruction);

    void testSet (uint32_t instruction);

    void test (uint32_t instruction);

    void _compare (uint32_t instruction, LuaValueCompare op);

    void add (uint32_t instruction);  // +
    void sub (uint32_t instruction); // -
    void mul (uint32_t instruction);  // *
    void mod (uint32_t instruction);  // %
    void pow (uint32_t instruction);  // ^
    void div (uint32_t instruction); // /
    void idiv (uint32_t instruction); // //
    void band (uint32_t instruction); // &
    void bor (uint32_t instruction); // |
    void bxor (uint32_t instruction); // ~
    void shl (uint32_t instruction);  // <<
    void shr (uint32_t instruction); // >>
    void unm (uint32_t instruction);  // -
    void bnot (uint32_t instruction);  //

    void eq (uint32_t instruction);

    void lt (uint32_t instruction);

    void le (uint32_t instruction);

    void forPrep (uint32_t instruction);

    void forLoop (uint32_t instruction);

    void printStack ();

    void setTop (int n);
};

#endif //LUACPP_LUAVM_H

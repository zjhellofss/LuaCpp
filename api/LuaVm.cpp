//
// Created by fushenshen on 2020/1/8.
//

#include "LuaVm.h"
#include "LuaState.h"
#include "../binchunk/Instruction.h"

extern const std::vector<OpcodeStruct> OpcodeStructVec;

//取出当前的指令
uint32_t LuaVm::fetch () {
    ProtoType *protoType = this->luaState.getProtoType();
    uint32_t i = protoType->code[this->Pc()];
    this->addPc(1);
    return i;
}

//
void LuaVm::addPc (int n) {
    int pc = this->luaState.getPc();
    this->luaState.setPc(pc + n);
}

void LuaVm::getConst (int idx) {
    ProtoType *p = this->getProtoType();
    auto c = p->constants[idx];
    int type = c->type;
    LuaValue luaValue;
    if (type == TAG_NUMBER) {
        luaValue = LuaValue(DOUBLE_TYPE, c->val);
    } else if (type == TAG_INTEGER) {
        luaValue = LuaValue(INT_TYPE, c->val);
    } else if (type == TAG_LONG_STR || type == TAG_SHORT_STR) {
        luaValue = LuaValue(STRING_TYPE, c->val);
    } else if (type == TAG_BOOLEAN) {
        luaValue = LuaValue(BOOL_TYPE, c->val);
    } else {
        //tag nil
        luaValue = LuaValue(NIL_TYPE, nullptr);
    }
    this->luaState.luaStack.push(luaValue);
}

//将常量值推入栈顶或者将索引值推入到栈中
void LuaVm::getRk (int rk) {
    if (rk > 0xFF) {
        this->getConst(rk & 0xFF);
    } else {
        this->luaState.pushValue(rk + 1);
    }
}

int LuaVm::Pc () {
    return this->luaState.getPc();
}


ProtoType *LuaVm::getProtoType () {
    return this->luaState.getProtoType();
}

void LuaVm::move (uint32_t instruction) {
    int a, b;
    std::tie(a, b, std::ignore) = ABC(instruction);
    a += 1;
    b += 1;
    this->luaState.copy(b, a);
}

void LuaVm::jmp (uint32_t instruction) {
    int a, b;
    std::tie(a, b) = AsBx(instruction);
    this->addPc(b);
    if (a != 0) {
        throw std::runtime_error("todo jmp");
    }
}

void LuaVm::loadNil (uint32_t instruction) {
    int a, b;
    std::tie(a, b, std::ignore) = ABC(instruction);
    a += 1;
    this->luaState.pushNil();
    for (int i = 1; i <= a + b; ++i) {
        //将栈顶的nil值复制到a-b的位置
        this->luaState.copy(-1, i);
    }
    //弹出栈顶的null值
    this->luaState.pop(1);
}

void LuaVm::loadBool (uint32_t instruction) {
    int a, b, c;
    std::tie(a, b, c) = ABC(instruction);
    a += 1;
    //luaState中的idx参数总是需要将传入值加1
    this->luaState.pushBoolean(b != 0);
    this->luaState.replace(a);
    if (c != 0) {
        this->addPc(1);
    }
}

void LuaVm::loadK (uint32_t instruction) {
    int a, bx;
    std::tie(a, bx) = ABX(instruction);
    a += 1;
    this->getConst(bx);
    //将其置换到指定位置
    this->luaState.replace(a);
}

void LuaVm::loadKx (uint32_t instruction) {
    int a, b;
    std::tie(a, b) = ABX(instruction);
    a += 1;
    int ax = AX(this->fetch());
    this->getConst(ax);
    this->luaState.replace(a);
}

void LuaVm::_binaryArith (uint32_t instruction, LuaValueOperator op) {
    int a, b, c;
    std::tie(a, b, c) = ABC(instruction);
    //获取两个常量值
    a += 1;
    this->getRk(b);
    this->getRk(c);
    this->luaState.Arith(op);
    this->luaState.replace(a);
}

void LuaVm::_unaryArith (uint32_t instruction, LuaValueOperator op) {
    int a, b;
    std::tie(a, b, std::ignore) = ABC(instruction);
    a += 1;
    b += 1;
    this->luaState.pushValue(b);
    this->luaState.Arith(op);
    this->luaState.replace(a);
}

void LuaVm::_len (uint32_t instruction) {
    int a, b;
    std::tie(a, b, std::ignore) = ABC(instruction);
    a += 1;
    b += 1;
    this->luaState.len(b);
    this->luaState.replace(a);
}

void LuaVm::concat (uint32_t instruction) {
    int a, b, c;
    std::tie(a, b, c) = ABC(instruction);
    a += 1;
    b += 1;
    c += 1;
    int n = c - b + 1;
    for (int i = b; i <= c; ++i) {
        this->luaState.pushValue(i);
    }
    this->luaState.concat(n);
    this->luaState.replace(a);
}

void LuaVm::_not (uint32_t instruction) {
    int a, b;
    std::tie(a, b, std::ignore) = ABC(instruction);
    a += 1;
    b += 1;
    this->luaState.pushValue(!this->luaState.toBoolean(b));
    this->luaState.replace(a);
}

void LuaVm::testSet (uint32_t instruction) {
    int a, b, c;
    std::tie(a, b, c) = ABC(instruction);
    a += 1;
    b += 1;
    bool l = this->luaState.toBoolean(b);
    if (l == (b != 0)) {
        this->luaState.copy(b, a);
    } else {
        this->addPc(1);
    }
}

void LuaVm::test (uint32_t instruction) {
    int a, c;
    std::tie(a, std::ignore, c) = ABC(instruction);
    a += 1;
    if (this->luaState.toBoolean(a) != (c != 0)) {
        this->addPc(1);
    }
}


void LuaVm::add (uint32_t instruction) {
    _binaryArith(instruction, LUA_OPADD);// +
}

void LuaVm::sub (uint32_t instruction) {
    _binaryArith(instruction, LUA_OPSUB); // -
}

void LuaVm::mul (uint32_t instruction) {
    _binaryArith(instruction, LUA_OPMUL);
}


void LuaVm::mod (uint32_t instruction) { _binaryArith(instruction, LUA_OPMOD); }  // %
void LuaVm::pow (uint32_t instruction) { _binaryArith(instruction, LUA_OPPOW); }  // ^
void LuaVm::div (uint32_t instruction) { _binaryArith(instruction, LUA_OPDIV); }  // /
void LuaVm::idiv (uint32_t instruction) { _binaryArith(instruction, LUA_OPIDIV); } // //
void LuaVm::band (uint32_t instruction) { _binaryArith(instruction, LUA_OPBAND); } // &
void LuaVm::bor (uint32_t instruction) { _binaryArith(instruction, LUA_OPBOR); }  // |
void LuaVm::bxor (uint32_t instruction) { _binaryArith(instruction, LUA_OPBXOR); } // ~
void LuaVm::shl (uint32_t instruction) { _binaryArith(instruction, LUA_OPSHL); }  // <<
void LuaVm::shr (uint32_t instruction) { _binaryArith(instruction, LUA_OPSHR); }  // >>
void LuaVm::unm (uint32_t instruction) { _unaryArith(instruction, LUA_OPUNM); }   // -
void LuaVm::bnot (uint32_t instruction) { _unaryArith(instruction, LUA_OPBNOT); }

void LuaVm::_compare (uint32_t instruction, LuaValueCompare op) {
    int a, b, c;
    std::tie(a, b, c) = ABC(instruction);
    this->getRk(b);
    this->getRk(c);
    if (this->luaState.compare(-2, -1, op) != (a != 0)) {
        this->addPc(1);
    }
    this->luaState.pop(2);
}

void LuaVm::eq (uint32_t instruction) {
    return _compare(instruction, LUA_OPEQ);
}

void LuaVm::lt (uint32_t instruction) {
    return _compare(instruction, LUA_OPLT);
}

void LuaVm::le (uint32_t instruction) {
    return _compare(instruction, LUA_OPLE);
}

void LuaVm::forPrep (uint32_t instruction) {
    int a, sbx;
    std::tie(a, sbx) = AsBx(instruction);
    a += 1;
    this->luaState.pushValue(a);
    this->luaState.pushValue(a + 2);
    this->luaState.Arith(LUA_OPSUB);
    this->luaState.replace(a);
    this->addPc(sbx);
}

void LuaVm::forLoop (uint32_t instruction) {
    int a, sbx;
    std::tie(a, sbx) = AsBx(instruction);
    a += 1;
    this->luaState.pushValue(a + 2);
    this->luaState.pushValue(a);
    this->luaState.Arith(LUA_OPADD);
    this->luaState.replace(a);
    bool isPositive = this->luaState.toNumber(a + 2) >= 0;
    if ((isPositive && this->luaState.compare(a, a + 1, LUA_OPLE)) ||
        (!isPositive && this->luaState.compare(a + 1, a, LUA_OPLE))) {
        this->addPc(sbx);
        this->luaState.copy(a, a + 3);
    }
}

LuaVm::LuaVm (ProtoType *protoType) : luaState(protoType) {}

void LuaVm::printStack () {
    return this->luaState.printLuaState();
}

void LuaVm::setTop (int n) {
    return this->luaState.setTop(n);
}









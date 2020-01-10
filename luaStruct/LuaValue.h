//
// Created by fushenshen on 2020/1/8.
//

#ifndef LUACPP_LUAVALUE_H
#define LUACPP_LUAVALUE_H

#include <string>
#include <vector>
#include <map>
#include "LuaMath.h"

//lua中的基础类型
enum LuaStateType {
    LUA_TNONE = -1,
    LUA_TNIL,
    LUA_TBOOLEAN,
    LUA_TLIGHTUSERDATA,
    LUA_TNUMBER,
    LUA_TINTEGER,
    LUA_TSTRING,
    LUA_TTABLE,
    LUA_TFUNCTION,
    LUA_TUSERDATA,
    LUA_TTHREAD,
    LUA_UNKOWNTYPE
};

#define BOOL_TYPE  0X01
#define NIL_TYPE 0X02
#define INT_TYPE 0X03
#define DOUBLE_TYPE 0X04
#define STRING_TYPE 0X05
#define TABLE_TYPE 0x06

enum LuaValueOperator {
    LUA_OPADD = 0, // +
    LUA_OPSUB,    // -
    LUA_OPMUL,   // *
    LUA_OPMOD,  // %
    LUA_OPPOW, // ^
    LUA_OPDIV,  // /
    LUA_OPIDIV,   // //
    LUA_OPBAND,   // &
    LUA_OPBOR,   // |
    LUA_OPBXOR,   // ~
    LUA_OPSHL,  // <<
    LUA_OPSHR, // >>
    LUA_OPUNM,// -
    LUA_OPBNOT,// ~
};

enum LuaValueCompare {
    LUA_OPEQ,
    LUA_OPLT,    // <
    LUA_OPLE,      // <=
};


//对于LuaValue的操作
inline int64_t iadd (int64_t a, int64_t b) {
    return a + b;
}

inline double fadd (double a, double b) {
    return a + b;
}

inline int64_t isub (int64_t a, int64_t b) {
    return a - b;
}

inline double fsub (double a, double b) {
    return a - b;
}

inline int64_t imul (int64_t a, int64_t b) {
    return a * b;
}

inline double fmul (double a, double b) {
    return a * b;
}

//imod
inline int64_t imod (int64_t a, int64_t b) {
    return iMod(a, b);
}

//fmod
inline double fmod (double a, double b) {
    return fMod(a, b);
}


//pow = std::pow

//iidiv
inline int64_t iidiv (int64_t a, int64_t b) {
    return iFloorDiv(a, b);
}

//fidiv
inline double fidiv (double a, double b) {
    return FFloorDiv(a, b);
}

inline int64_t shl (int64_t a, int64_t b) {
    return shiftLeft(a, b);
}

inline int64_t shr (int64_t a, int64_t b) {
    return shiftRight(a, b);
}

inline double div (double a, double b) {
    return a / b;
}

inline int64_t band (int64_t a, int64_t b) {
    return a & b;
}

inline int64_t bor (int64_t a, int64_t b) {
    return a | b;
}

inline int64_t bxor (int64_t a, int64_t b) {
    return a ^ b;
}


inline int64_t iunm (int64_t a, int64_t b) {
    return -a;
}

inline double funm (double a, double b) {
    return a;
}

inline int64_t bnot (int64_t a, int64_t b) {
    return ~a;
}

class LuaValue {
private:
    int type = NIL_TYPE;
    void *val = nullptr;
    //LuaTable 结构专用
public:
    int getType () const;

    void setType (int type);

    void *getVal () const;

    void setVal (void *val);

    LuaValue ();

    LuaValue (int type, void *val);

    LuaValue (const LuaValue &luaValue);

    LuaValue &operator= (const LuaValue &luaValue);


    virtual ~ LuaValue ();


    std::pair<int, bool> convertToInteger () const;

    std::pair<double, bool> convertToFloat () const;

    bool operator< (const LuaValue &rhs) const;

};

class Operator {
private:
    int64_t (*integerFunc) (int64_t, int64_t);

    double (*floatFunc  ) (double, double);

public:
    friend LuaValue _arith (LuaValue &a, LuaValue &b, const Operator &op);

    Operator (int64_t (*integerFunc) (int64_t, int64_t), double (*floatFunc) (double, double)) : integerFunc(
            integerFunc), floatFunc(floatFunc) {}

};


LuaStateType typeOf (LuaValue luaValue);

LuaValue _arith (LuaValue &a, LuaValue &b, const Operator &op);

bool _lt (LuaValue &a, LuaValue &b);

bool _le (LuaValue &a, LuaValue &b);


bool _eq (LuaValue a, LuaValue b);

#endif //LUACPP_LUAVALUE_H

//
// Created by fushenshen on 2020/1/8.
//

#ifndef LUACPP_LUAVALUE_H
#define LUACPP_LUAVALUE_H

#include <string>
#include <vector>

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
public:
    int getType () const;

    void setType (int type);

    void *getVal () const;

    void setVal (void *val);

    LuaValue ();

    LuaValue (int type, void *val);

    LuaValue (const LuaValue &luaValue);

    LuaValue &operator= (const LuaValue &luaValue) {
        this->type = luaValue.type;
        switch (type) {
            case NIL_TYPE: {
                this->val = nullptr;
                break;
            }
            case BOOL_TYPE: {
                bool b = *reinterpret_cast<bool *> (luaValue.val);
                this->val = new bool(b);
                break;
            }
            case INT_TYPE: {
                int64_t v = *reinterpret_cast<int64_t *>(luaValue.val);
                this->val = new int64_t(v);
                break;
            }
            case DOUBLE_TYPE: {
                double v = *reinterpret_cast<double *>(luaValue.val);
                this->val = new double(v);
                break;
            }
            case STRING_TYPE: {
                std::string v = *reinterpret_cast<std::string *>(luaValue.val);
                this->val = new std::string(v);
                break;
            }
            default: {
                break;
            }
        }
        return *this;
    }


    virtual ~

    LuaValue () {
        if (val != nullptr) {
            switch (type) {
                case NIL_TYPE: {
                    break;
                }
                case BOOL_TYPE: {
                    delete reinterpret_cast<bool *>( val);
                    break;
                }
                case INT_TYPE: {
                    delete reinterpret_cast<int64_t *>(val);
                    break;
                }
                case DOUBLE_TYPE: {
                    delete reinterpret_cast<double *>(val);
                    break;
                }
                case STRING_TYPE: {
                    delete reinterpret_cast<std::string *>(val);
                    break;
                }
                default: {
                    break;
                }
            }
            val = nullptr;
        }
    }

    std::pair<int64_t, bool> convertToInteger ();

    std::pair<double, bool> convertToFloat ();
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


bool _eq (LuaValue &a, LuaValue &b);

#endif //LUACPP_LUAVALUE_H

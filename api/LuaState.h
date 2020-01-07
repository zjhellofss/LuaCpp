//
// Created by fushenshen on 2020/1/7.
//

#ifndef LUACPP_LUATYPE_H
#define LUACPP_LUATYPE_H

#include <deque>
#include <string>
#include <cassert>

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

struct LuaValue {
    int type = NIL_TYPE;
    void *val = nullptr;

    LuaValue () : type(NIL_TYPE), val(nullptr) {
    }

    LuaValue (int type, void *val) : type(type), val(val) {}

    LuaValue (const LuaValue &luaValue) {
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
                int v = *reinterpret_cast<int *>(luaValue.val);
                this->val = new int(v);
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
    }

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
                int v = *reinterpret_cast<int *>(luaValue.val);
                this->val = new int(v);
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


    virtual ~LuaValue () {
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
                    delete reinterpret_cast<int *>(val);
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


};

inline LuaStateType typeOf (LuaValue luaValue) {
    switch (luaValue.type) {
        case NIL_TYPE:
            return LUA_TNIL;
        case BOOL_TYPE:
            return LUA_TBOOLEAN;
        case INT_TYPE:
            return LUA_TINTEGER;
        case DOUBLE_TYPE:
            return LUA_TNUMBER;
        case STRING_TYPE:
            return LUA_TSTRING;
        default:
            return LUA_UNKOWNTYPE;
    }
}

struct LuaStack {
public:
    std::deque<LuaValue> stk;

public:

    void check (int n) {
        //检查Luastack是否还可以容纳n个值
    }

    void push (const LuaValue &v) {
        stk.push_back(v);
    }

    LuaValue top () {
        auto v = stk.back();
        return v;
    }

    void pop () {
        this->stk.pop_back();
    }

    int absIndex (int idx) {
        if (idx > 0) {
            return idx;
        } else {
            return static_cast<int>(idx + this->stk.size() + 1);
        }
    }

    //luaStack的指针从1开始
    bool isValid (int idx) {
        idx = this->absIndex(idx);
        return idx > 0 && idx <= stk.size();
    }

    LuaValue get (int idx) {
        idx = this->absIndex(idx);
        if (isValid(idx)) {
            return this->stk[idx - 1];
        } else {
            throw std::range_error("range error in get method");
        }
    }

    void set (int idx, const LuaValue &luaValue) {
        idx = this->absIndex(idx);
        if (isValid(idx)) {
            this->stk[idx - 1] = luaValue;
        } else {
            throw std::range_error("range error in set method");
        }
    }
};

struct LuaState {
public:
    LuaValue getTop ();

    int absIndex (int idx);

    bool checkStack ();

    void copy (int fromIdx, int toIdx);

    void pushValue (int idx);

    void replace (int idx);

    void rotate (int idx, int n);

    void insert (int idx);

    void remove (int idx);

    void pushNil ();

    void printLuaState ();

    int stkSize () const;

    std::string toString (int idx);

    double toNumber (int idx);

    int toInteger (int idx);

    bool isInteger (int idx);

    bool toBoolean (int idx);

    void setTop (int idx);

    void pushBoolean (bool b);

    void pushInteger (int v);

    void pushNumber (double v);

    void pushString (std::string s);

    void pop (int n);

    LuaStateType type (int idx);


    bool isNone (int idx);

    bool isNil (int idx);

    bool isNoneOrNil (int idx);

    bool isBoolean (int idx);

    bool isString (int idx);

    std::string typeName (const LuaStateType &type);


private:
    LuaStack luaStack;

};


#endif //LUACPP_LUATYPE_H

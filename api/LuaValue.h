//
// Created by fushenshen on 2020/1/8.
//

#ifndef LUACPP_LUAVALUE_H
#define LUACPP_LUAVALUE_H

#include <string>

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
public:
    int type = NIL_TYPE;
    void *val = nullptr;
public:
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

    std::pair<int, bool> convertToInteger (const LuaValue &luaValue);

    std::pair<double, bool> convertToFloat (const LuaValue &luaValue);
};

LuaStateType typeOf (LuaValue luaValue);


#endif //LUACPP_LUAVALUE_H

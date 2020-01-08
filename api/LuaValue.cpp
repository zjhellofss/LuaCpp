//
// Created by fushenshen on 2020/1/8.
//

#include "LuaValue.h"
#include <string>
#include "LuaMath.h"

LuaValue::LuaValue () : type(NIL_TYPE), val(nullptr) {
}

LuaValue::LuaValue (int type, void *val) : type(type), val(val) {}

LuaValue::LuaValue (const LuaValue &luaValue) {
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


std::pair<double, bool> LuaValue::convertToFloat () {
    if (this->type == DOUBLE_TYPE) {
        double v = *reinterpret_cast<double *>(this->val);
        return {v, true};
    } else if (this->type == STRING_TYPE) {
        const std::string &s = *reinterpret_cast<std::string *>(this->val);
        return parseDouble(s);
    } else if (this->type == INT_TYPE) {
        double d = *reinterpret_cast<int64_t *>(this->val);
        return {d, true};
    } else {
        return {0.0, false};
    }
}

std::pair<int64_t, bool> LuaValue::convertToInteger () {
    int t = this->type;
    if (t == INT_TYPE) {
        return {*reinterpret_cast<int64_t *>(this->val), true};
    } else if (t == DOUBLE_TYPE) {
        return floatToInteger(*reinterpret_cast<double *>(this->val));
    } else if (t == STRING_TYPE) {
        const std::string &s = *reinterpret_cast<std::string *>(this->val);
        return parseInteger(s);
    } else {
        return {0, false};
    }
}

LuaStateType typeOf (LuaValue luaValue) {
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
//
// Created by fushenshen on 2020/1/7.
//

#include "LuaState.h"

LuaValue LuaState::getTop () {
    return this->luaStack.stk.back();
}

int LuaState::absIndex (int idx) {
    return this->luaStack.absIndex(idx);
}

bool LuaState::checkStack () {
    return true;
}

void LuaState::pop (int n) {
    for (int i = 0; i < n; ++i) {
        this->luaStack.pop();
    }
}

void LuaState::copy (int fromIdx, int toIdx) {
    LuaValue luaValue = this->luaStack.get(fromIdx);
    this->luaStack.set(toIdx, luaValue);
}

void LuaState::pushValue (int idx) {
    LuaValue luaValue = this->luaStack.get(idx);
    this->luaStack.push(luaValue);
}

void LuaState::replace (int idx) {
    LuaValue luaValue = this->luaStack.top();
    this->luaStack.pop();
    this->luaStack.set(idx, luaValue);
}

void LuaState::rotate (int idx, int n) {
    idx = this->absIndex(idx);
    if (this->luaStack.isValid(idx)) {
        auto &stk = this->luaStack.stk;
        auto p = stk.begin() + idx;
        if (std::abs(n) == 1) {
            if (n == 1) {
                std::__rotate_right(p, stk.end());
            } else {
                std::__rotate_left(p, stk.end());
            }
        }
    }
}

void LuaState::insert (int idx) {
    idx -= 1;
    idx = this->absIndex(idx);
    if (this->luaStack.isValid(idx)) {
        this->rotate(idx, 1);
    } else {
        throw std::range_error("range error in insert");
    }
}

void LuaState::remove (int idx) {
    idx -= 1;
    idx = this->absIndex(idx);
    if (this->luaStack.isValid(idx)) {
        auto &stk = this->luaStack.stk;
        stk.erase(stk.begin() + idx, stk.begin() + idx + 1);
    } else {
        throw std::range_error("range error in remove");
    }
}


void LuaState::pushNil () {
    this->luaStack.push(LuaValue());
}


void LuaState::setTop (int idx) {
    idx = this->absIndex(idx);
    if (idx < 0) {
        throw std::range_error("range error in setTop");
    }
    int s = this->stkSize();
    if (idx <= s) {
        this->pop(s - idx);
    } else {
        while (idx - s > 0) {
            this->pushNil();
            s++;
        }
    }
}

void LuaState::pushBoolean (bool b) {
    auto l = LuaValue(BOOL_TYPE, new bool(b));
    this->luaStack.push(l);
}

void LuaState::pushInteger (int v) {
    this->luaStack.push(LuaValue(INT_TYPE, new int(v)));
}

void LuaState::pushNumber (double v) {
    this->luaStack.push(LuaValue(DOUBLE_TYPE, new double(v)));
}

void LuaState::pushString (std::string s) {
    this->luaStack.push(LuaValue(STRING_TYPE, new std::string(std::move(s))));
}

std::string LuaState::typeName (const LuaStateType &type) {
    switch (type) {
        case LUA_TNONE:
            return "no value";
        case LUA_TNIL:
            return "nil";
        case LUA_TBOOLEAN:
            return "boolean";
        case LUA_TNUMBER:
            return "number";
        case LUA_TSTRING:
            return "string";
        case LUA_TTABLE:
            return "table";
        case LUA_TFUNCTION:
            return "function";
        case LUA_TTHREAD:
            return "thread";
        default:
            return "userdata";
    }
}

LuaStateType LuaState::type (int idx) {
    idx = this->absIndex(idx);
    if (this->luaStack.isValid(idx)) {
        auto v = this->luaStack.get(idx);
        return typeOf(v);
    } else {
        throw std::range_error("range error in method Type");
    }
}

bool LuaState::isNone (int idx) {
    return this->type(idx) == LUA_TNONE;
}

bool LuaState::isNil (int idx) {
    return this->type(idx) == LUA_TNIL;
}

bool LuaState::isNoneOrNil (int idx) {
    return this->type(idx) <= LUA_TNIL;
}

bool LuaState::isBoolean (int idx) {
    return this->type(idx) == LUA_TBOOLEAN;
}

bool LuaState::isString (int idx) {
    int type = this->luaStack.get(idx).type;
    return type == LUA_TSTRING || type == LUA_TNUMBER;
}

bool LuaState::isInteger (int idx) {
    auto v = this->luaStack.get(idx);
    int t = v.type;
    return t == LUA_TINTEGER;
}

bool LuaState::toBoolean (int idx) {
    auto v = this->luaStack.get(idx);
    int type = v.type;
    if (type == LUA_TNIL || type == LUA_TNONE) {
        return false;
    } else if (type == LUA_TBOOLEAN) {
        bool b = *reinterpret_cast<bool *>( v.val);
        return b;
    } else {
        return true;
    }
}

double LuaState::toNumber (int idx) {
    auto v = this->luaStack.get(idx);
    int type = v.type;
    assert(type == INT_TYPE || type == DOUBLE_TYPE);
    auto res = v.convertToFloat();
    bool f = res.second;
    if (f) {
        return res.first;
    } else {
        throw std::logic_error("toNumber");
    }
}

int LuaState::toInteger (int idx) {
    auto v = this->luaStack.get(idx);
    int type = v.type;
    assert(type == INT_TYPE);
    auto res = v.convertToInteger();
    bool f = res.second;
    if (f) {
        return res.first;
    } else {
        throw std::logic_error("toInteger");
    }
}

std::string LuaState::toString (int idx) {
    auto v = this->luaStack.get(idx);
    if (v.type == STRING_TYPE) {
        return std::string(*reinterpret_cast<std::string *> (v.val));
    } else if (v.type == INT_TYPE) {
        return std::to_string(*reinterpret_cast<int *>(v.val));
    } else if (v.type == DOUBLE_TYPE) {
        return std::to_string(*reinterpret_cast<double *> (v.val));
    } else {
        return "";
    }
}

int LuaState::stkSize () const {
    return static_cast<int>(luaStack.stk.size());
}

void LuaState::printLuaState () {
    auto s = this->stkSize();
    for (int i = 1; i <= s; ++i) {
        LuaStateType type = this->type(i);
        if (type == LUA_TBOOLEAN) {
            bool b = this->toBoolean(i);
            printf("[boolean:%d]", b);
        } else if (type == LUA_TNUMBER) {
            printf("[%f]", this->toNumber(i));
        } else if (type == LUA_TINTEGER) {
            printf("[%d]", this->toInteger(i));
        } else if (type == LUA_TSTRING) {
            printf("[%s]", this->toString(i).data());
        } else {
            printf("[%s]", this->typeName(type).data());
        }
    }
    printf("\n");
}


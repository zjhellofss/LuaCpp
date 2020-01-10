//
// Created by fushenshen on 2020/1/8.
//

#include "LuaValue.h"
#include <string>
#include "LuaMath.h"
#include "LuaTable.h"

LuaValue::LuaValue () : type(NIL_TYPE), val(nullptr) {
}

LuaValue::LuaValue (int type, void *val) : type(type) {
    switch (type) {
        case NIL_TYPE: {
            this->val = nullptr;
            break;
        }
        case BOOL_TYPE: {
            bool b = *reinterpret_cast<bool *> (val);
            this->val = new bool(b);
            break;
        }
        case INT_TYPE: {
            int v = *reinterpret_cast<int *>(val);
            this->val = new int(v);
            break;
        }
        case DOUBLE_TYPE: {
            double v = *reinterpret_cast<double *>(val);
            this->val = new double(v);
            break;
        }
        case STRING_TYPE: {
            std::string v = *reinterpret_cast<std::string *>(val);
            this->val = new std::string(v);
            break;
        }
        case TABLE_TYPE: {
            LuaTable table = *reinterpret_cast<LuaTable *>(val);
            this->val = new LuaTable(table);
            break;
        }
        default: {
            break;
        }
    }
}

LuaValue::LuaValue (const LuaValue &luaValue) {
    this->type = luaValue.getType();
    switch (type) {
        case NIL_TYPE: {
            this->val = nullptr;
            break;
        }
        case BOOL_TYPE: {
            bool b = *reinterpret_cast<bool *> (luaValue.getVal());
            this->val = new bool(b);
            break;
        }
        case INT_TYPE: {
            int v = *reinterpret_cast<int *>(luaValue.getVal());
            this->val = new int(v);
            break;
        }
        case DOUBLE_TYPE: {
            double v = *reinterpret_cast<double *>(luaValue.getVal());
            this->val = new double(v);
            break;
        }
        case STRING_TYPE: {
            std::string v = *reinterpret_cast<std::string *>(luaValue.getVal());
            this->val = new std::string(v);
            break;
        }
        case TABLE_TYPE: {
            LuaTable table = *reinterpret_cast<LuaTable *>(val);
            this->val = new LuaTable(table);
            break;
        }
        default: {
            break;
        }
    }
}


std::pair<double, bool> LuaValue::convertToFloat () const {
    if (this->type == DOUBLE_TYPE) {
        double v = *reinterpret_cast<double *>(this->val);
        return {v, true};
    } else if (this->type == STRING_TYPE) {
        const std::string &s = *reinterpret_cast<std::string *>(this->val);
        return parseDouble(s);
    } else if (this->type == INT_TYPE) {
        double d = *reinterpret_cast<int *>(this->val);
        return {d, true};
    } else {
        return {0.0, false};
    }
}

std::pair<int, bool> LuaValue::convertToInteger () const {
    int t = this->type;
    if (t == INT_TYPE || t == BOOL_TYPE) {
        return {*reinterpret_cast<int *>(this->val), true};
    } else if (t == DOUBLE_TYPE) {
        return floatToInteger(*reinterpret_cast<double *>(this->val));
    } else if (t == STRING_TYPE) {
        const std::string &s = *reinterpret_cast<std::string *>(this->val);
        return parseInteger(s);
    } else {
        return {0, false};
    }
}

int LuaValue::getType () const {
    return type;
}

void LuaValue::setType (int type) {
    LuaValue::type = type;
}

void *LuaValue::getVal () const {
    return val;
}

void LuaValue::setVal (void *val) {
    LuaValue::val = val;
}

bool LuaValue::operator< (const LuaValue &rhs) const {
    if (type == rhs.type) {
        if (type == STRING_TYPE) {
            return *reinterpret_cast<std::string *>(this->val)
                   < *reinterpret_cast<std::string *>(rhs.val);
        } else if (type == INT_TYPE || type == BOOL_TYPE) {
            auto l = this->convertToInteger();
            auto r = rhs.convertToInteger();
            if (l.second && r.second) {
                return l.first < r.first;
            } else {
                return false;
            }
        } else if (type == DOUBLE_TYPE) {
            auto l = this->convertToFloat();
            auto r = rhs.convertToFloat();
            if (l.second && r.second) {
                return l.first < r.first;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else if (type == TABLE_TYPE) {
        auto *t1 = reinterpret_cast<LuaTable *>(this->val);
        auto *t2 = reinterpret_cast<LuaTable *>(rhs.val);
        return t1->size() < t2->size();
    } else {
        return false;
    }
}

LuaValue &LuaValue::operator= (const LuaValue &luaValue) {
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
            int64_t v = *reinterpret_cast<int *>(luaValue.val);
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
        case TABLE_TYPE: {
            LuaTable table = *reinterpret_cast<LuaTable *>(val);
            this->val = new LuaTable(table);
            break;
        }
        default: {
            break;
        }
    }
    return *this;

}

LuaValue::~LuaValue () {

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
            case TABLE_TYPE: {
                delete reinterpret_cast<LuaTable *>(val);
                break;
            }
            default: {
                break;
            }
        }
        val = nullptr;
    }
}


LuaStateType typeOf (LuaValue luaValue) {
    switch (luaValue.getType()) {
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
        case TABLE_TYPE:
            return LUA_TTABLE;
        default:
            return LUA_UNKOWNTYPE;
    }
}

bool _le (LuaValue &a, LuaValue &b) {
    int t1 = a.getType();
    int t2 = b.getType();
    if (t1 == STRING_TYPE) {
        if (t2 == STRING_TYPE) {
            auto s1 = *(reinterpret_cast<std::string *>(a.getVal()));
            auto s2 = *(reinterpret_cast<std::string *>(b.getVal()));
            return s1 < s2;
        }
    } else if (t1 == INT_TYPE || t1 == DOUBLE_TYPE) {
        if (t2 == INT_TYPE || t2 == DOUBLE_TYPE) {
            auto v1 = a.convertToFloat();
            auto v2 = b.convertToFloat();
            //如_eq表示均转换成功
            if (v1.second && v2.second) {
                return v1.first <= v2.first;
            } else {
                return false;
            }
        }
    }
    return false;
}

bool _lt (LuaValue &a, LuaValue &b) {
    int t1 = a.getType();
    int t2 = b.getType();
    if (t1 == STRING_TYPE) {
        if (t2 == STRING_TYPE) {
            auto s1 = *(reinterpret_cast<std::string *>(a.getVal()));
            auto s2 = *(reinterpret_cast<std::string *>(b.getVal()));
            return s1 < s2;
        }
    } else if (t1 == INT_TYPE || t1 == DOUBLE_TYPE) {
        if (t2 == INT_TYPE || t2 == DOUBLE_TYPE) {
            auto v1 = a.convertToFloat();
            auto v2 = b.convertToFloat();
            //如_eq表示均转换成功
            if (v1.second && v2.second) {
                return v1.first < v2.first;
            } else {
                return false;
            }
        }
    }
    return false;

}

bool _eq (LuaValue a, LuaValue b) {
    int t1 = a.getType();
    int t2 = b.getType();
    if (t1 == NIL_TYPE) {
        return b.getType() == NIL_TYPE;
    } else if (t1 == BOOL_TYPE) {
        if (t2 == BOOL_TYPE) {
            auto b1 = *(reinterpret_cast<bool *>(a.getVal()));
            auto b2 = *(reinterpret_cast<bool *>(b.getVal()));
            return b1 == b2;
        }
    } else if (t1 == STRING_TYPE) {
        if (t2 == STRING_TYPE) {
            auto s1 = *(reinterpret_cast<std::string *>(a.getVal()));
            auto s2 = *(reinterpret_cast<std::string *>(b.getVal()));
            return s1 == s2;
        }
    } else if (t1 == INT_TYPE && t2 == INT_TYPE) {
        auto v1 = a.convertToInteger();
        auto v2 = b.convertToInteger();
        if (v1.second && v2.second) {
            return v1.first == v2.first;
        } else {
            return false;
        }
    } else if ((t1 == DOUBLE_TYPE && t2 == DOUBLE_TYPE)) {
        auto v1 = a.convertToFloat();
        auto v2 = b.convertToFloat();
        if (v1.second && v2.second) {
            return v1.first == v2.first;
        } else {
            return false;
        }
    } else if ((t1 == DOUBLE_TYPE && t2 == INT_TYPE)
               || (t1 == INT_TYPE && t2 == DOUBLE_TYPE)) {
        auto v1 = a.convertToFloat();
        auto v2 = b.convertToFloat();
        if (v1.second && v2.second) {
            return v1.first == v2.first;
        } else {
            return false;
        }
    }
    return false;
}

LuaValue _arith (LuaValue &a, LuaValue &b, const Operator &op) {
    if (op.floatFunc == nullptr) {
        auto p = a.convertToInteger();
        if (p.second) {
            auto q = a.convertToInteger();
            if (q.second) {
                auto v = op.integerFunc(p.first, q.first);
                return LuaValue(INT_TYPE, new int(v));
            }
        }
    } else {
        if (op.integerFunc != nullptr) {
            auto p = a.convertToInteger();
            auto q = b.convertToInteger();
            if (p.second) {
                if (q.second) {
                    return LuaValue(INT_TYPE, new int(op.integerFunc(p.first, q.first)));
                }
            }

        }
        //转为double值再进行计算
        auto p = a.convertToFloat();
        if (p.second) {
            auto q = b.convertToFloat();
            if (q.second) {
                return LuaValue(DOUBLE_TYPE, new double(op.floatFunc(p.first, q.first)));
            }
        }

    }
    return LuaValue{};
}


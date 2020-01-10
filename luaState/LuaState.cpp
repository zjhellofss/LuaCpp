//
// Created by fushenshen on 2020/1/7.
//

#include "LuaState.h"
#include "../luaStruct/LuaTable.h"

const std::vector<Operator> operators = std::vector<Operator>{
        Operator{iadd, fadd},
        Operator{isub, fsub},
        Operator{imul, fmul},
        Operator{imod, fmod},
        Operator{nullptr, pow},
        Operator{nullptr, div},
        Operator{iidiv, fidiv},
        Operator{band, nullptr},
        Operator{bor, nullptr},
        Operator{bxor, nullptr},
        Operator{shl, nullptr},
        Operator{shr, nullptr},
        Operator{iunm, funm},
        Operator{bnot, nullptr},
};

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
    this->luaStack.push(LuaValue(INT_TYPE, new int64_t(v)));
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
            return "no .getVal()ue";
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
    int type = this->luaStack.get(idx).getType();
    return type == LUA_TSTRING || type == LUA_TNUMBER;
}

bool LuaState::isInteger (int idx) {
    auto v = this->luaStack.get(idx);
    int t = v.getType();
    return t == LUA_TINTEGER;
}

bool LuaState::toBoolean (int idx) {
    auto v = this->luaStack.get(idx);
    int type = v.getType();
    if (type == LUA_TNIL || type == LUA_TNONE) {
        return false;
    } else if (type == LUA_TBOOLEAN) {
        bool b = *reinterpret_cast<bool *>( v.getVal());
        return b;
    } else {
        return true;
    }
}

double LuaState::toNumber (int idx) {
    auto v = this->luaStack.get(idx);
    int type = v.getType();
    assert(type == INT_TYPE || type == DOUBLE_TYPE);
    auto res = v.convertToFloat();
    bool f = res.second;
    if (f) {
        return res.first;
    } else {
        throw std::runtime_error("toNumber");
    }
}

int64_t LuaState::toInteger (int idx) {
    auto v = this->luaStack.get(idx);
    int type = v.getType();
    assert(type == INT_TYPE);
    auto res = v.convertToInteger();
    bool f = res.second;
    if (f) {
        return res.first;
    } else {
        throw std::runtime_error("toInteger");
    }
}

std::string LuaState::toString (int idx) {
    auto v = this->luaStack.get(idx);
    if (v.getType() == STRING_TYPE) {
        return std::string(*reinterpret_cast<std::string *> (v.getVal()));
    } else if (v.getType() == INT_TYPE) {
        return std::to_string(*reinterpret_cast<int *>(v.getVal()));
    } else if (v.getType() == DOUBLE_TYPE) {
        return std::to_string(*reinterpret_cast<double *> (v.getVal()));
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


void LuaState::len (int idx) {
    auto v = this->luaStack.get(idx);
    if (v.getType() == STRING_TYPE) {
        size_t l = ((std::string *) (v.getVal()))->size();
        this->luaStack.push(LuaValue(INT_TYPE, new int64_t(l)));
    } else if (v.getType() == TABLE_TYPE) {
        size_t l = reinterpret_cast<LuaTable *>(v.getVal())->arrLen();
        this->luaStack.push(LuaValue(INT_TYPE, new int(l)));
    } else {
        throw std::runtime_error("length error");
    }
}

void LuaState::concat (int n) {
    if (!n) {
        this->luaStack.push(LuaValue(STRING_TYPE, new std::string("")));
    } else if (n >= 2) {
        for (int i = 1; i < n; ++i) {
            if (this->isString(-1) && this->isString(-2)) {
                auto s1 = this->toString(-2);
                auto s2 = this->toString(-1);
                this->luaStack.pop();
                this->luaStack.pop();
                this->luaStack.push(LuaValue(STRING_TYPE, new std::string(s1 + s2)));
            } else {
                throw std::runtime_error("concat error");
            }
        }
    }
}

bool LuaState::compare (int idx1, int idx2, LuaValueCompare luaValueCompare) {
    auto a = this->luaStack.get(idx1);
    auto b = this->luaStack.get(idx2);
    if (luaValueCompare == LUA_OPEQ) {
        return _eq(a, b);
    } else if (luaValueCompare == LUA_OPLT) {
        return _lt(a, b);
    } else if (luaValueCompare == LUA_OPLE) {
        return _le(a, b);
    } else {
        throw std::runtime_error("in compare");
    }
}

void LuaState::Arith (LuaValueOperator op1) {
    auto b = this->luaStack.top();
    this->luaStack.pop();
    LuaValue a;
    if (op1 != LUA_OPUNM && op1 != LUA_OPBNOT) {
        a = this->luaStack.top();
        this->luaStack.pop();
    } else {
        a = b;
    }
    auto op = operators[op1];
    auto res = _arith(a, b, op);
    if (!res.getVal() && res.getType() == NIL_TYPE) {
        throw std::runtime_error("arithmetic error!");
    } else {
        this->luaStack.push(res);
    }
}

LuaState::LuaState (ProtoType *protoType, int pc) : protoType(protoType), pc(pc) {}

LuaState::~LuaState () {
    if (this->protoType != nullptr) {
        delete protoType;
        this->protoType = nullptr;
    }
}

ProtoType *LuaState::getProtoType () const {
    return protoType;
}

LuaState::LuaState (ProtoType *protoType) : protoType(protoType), pc(0) {}

int LuaState::getPc () const {
    return pc;
}

void LuaState::setPc (int pc) {
    LuaState::pc = pc;
}

void LuaState::createTable (int nArr, int Rec) {
    this->luaStack.push(LuaValue{TABLE_TYPE, new LuaTable()});
}

void LuaState::newTable () {
    this->createTable(0, 0);
}

LuaValue LuaState::getTable (int idx) {
    auto t1 = this->luaStack.get(idx);
    auto k = this->luaStack.top();
    this->luaStack.pop();
    auto table = reinterpret_cast<LuaTable *>(t1.getVal());
    return this->getTable_(table, k);
}

LuaValue LuaState::getTable_ (LuaTable *t, LuaValue k) {
    LuaValue val = *(t->get(&k));
    return val;
}

LuaValue LuaState::getField (int idx, const std::string &str) {
    this->pushString(str);
    return this->getTable(idx);
}

LuaValue LuaState::getI (int idx, int i) {
    LuaValue luaValue = this->luaStack.get(idx);
    auto *table = reinterpret_cast<LuaTable *>(luaValue.getVal());
    return this->getTable_(table, LuaValue(INT_TYPE, new int(i)));
}

void LuaState::setTable (int idx) {
    LuaTable *luaTable = reinterpret_cast<LuaTable *>(this->luaStack.get(idx).getVal());
    LuaValue key = this->luaStack.top();
    this->luaStack.pop();
    LuaValue value = this->luaStack.top();
    this->luaStack.pop();
    return this->setTable_(luaTable,
                           new LuaValue(key.getType(), key.getVal()), new LuaValue(value.getType(), value.getVal()));
}

void LuaState::setTable_ (LuaTable *table, LuaValue *k, LuaValue *v) {
    assert(table != nullptr);
    return table->put(k, v);
}

void LuaState::setField (int idx, const std::string &str) {
    auto *t = reinterpret_cast<LuaTable *>(this->luaStack.get(idx).getVal());
    auto v = this->luaStack.top();
    this->luaStack.pop();
    auto *value = new LuaValue(v);
    t->put(new LuaValue(STRING_TYPE, new std::string(str)), value);
}

void LuaState::setI (int idx, int i) {
    auto *t = reinterpret_cast<LuaTable *>(this->luaStack.get(idx).getVal());
    auto v_ = this->luaStack.top();
    auto v = new LuaValue(v_.getType(), v_.getVal());
    this->setTable_(t, new LuaValue(INT_TYPE, new int(i)), v);
}








//
// Created by fushenshen on 2020/1/7.
//

#ifndef LUACPP_LUATYPE_H
#define LUACPP_LUATYPE_H

#include <deque>
#include <string>
#include <cassert>

#include "LuaValue.h"
#include "../binchunk/BinaryChunk.h"

class LuaState;

class LuaStack {
private:
    std::deque<LuaValue> stk;

public:
    friend class LuaState;

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

class LuaState {
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

    int64_t toInteger (int idx);

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

    void len (int idx);

    void concat (int n);

    bool compare (int idx1, int idx2, LuaValueCompare luaValueCompare);

    void Arith (LuaValueOperator op1);


    virtual ~LuaState ();

public:
    LuaState (ProtoType *protoType, int pc);

    friend class LuaVm;

    ProtoType *getProtoType () const;

private:
    LuaStack luaStack;
    ProtoType *protoType;
    int pc;
};


#endif //LUACPP_LUATYPE_H

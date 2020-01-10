//
// Created by fushenshen on 2020/1/10.
//

#ifndef LUACPP_LUATABLE_H
#define LUACPP_LUATABLE_H

#include <vector>
#include <map>

class LuaValue;

class LuaTable {
private:
    std::vector<LuaValue *> arr;
    std::map<LuaValue *, LuaValue *> map_;
public:
    LuaValue *get (LuaValue *key);
    void put (LuaValue *key, LuaValue *value);
    int size() const;

    virtual ~LuaTable ();
    size_t  arrLen() const;
};

#endif //LUACPP_LUATABLE_H

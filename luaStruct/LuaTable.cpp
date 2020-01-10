//
// Created by fushenshen on 2020/1/10.
//

#include <cassert>
#include "LuaTable.h"
#include "LuaValue.h"

LuaValue *LuaTable::get (LuaValue *key) {
    auto p = key->convertToInteger();
    //可以转换为一个int值的key
    if (p.second) {
        int idx = p.first;
        if (idx >= 1 && idx <= this->arr.size()) {
            return this->arr[idx - 1];
        } else {
            throw std::runtime_error("table get");
        }
    } else {
        auto *p1 = this->map_.at(key);
        return p1;
    }
}

void LuaTable::put (LuaValue *key, LuaValue *value) {
    assert(key != nullptr);
    auto p = key->convertToInteger();
    if (p.second) {
        int idx = p.first;
        if (value != nullptr || value->getType() != NIL_TYPE) {
            if (idx >= this->arr.size()) {
                //扩充数组的大小
                this->arr.resize(idx + 1);
            }
            this->arr[idx] = value;
        }
    } else {
        if (value != nullptr) {
            this->map_.insert({key, value});
        } else {
            auto res = this->map_.find(key);
            if (res != this->map_.end()) {
                this->map_.erase(res);
            } else {
                throw std::runtime_error("map insert val");
            }
        }
    }
}

int LuaTable::size () const {
    return static_cast<int>(this->map_.size() + this->arr.size());
}

LuaTable::~LuaTable () {
    for (auto &c :this->arr) {
        if (c != nullptr) {
            delete c;
            c = nullptr;
        }
    }
    for (auto &p:this->map_) {
        delete p.first;
        delete p.second;
    }
}

size_t LuaTable::arrLen () const {
    return this->arr.size();
}


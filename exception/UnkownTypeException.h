//
// Created by fushenshen on 2020/1/6.
//

#ifndef LUACPP_UNKOWNTYPEEXCEPTION_H
#define LUACPP_UNKOWNTYPEEXCEPTION_H

#include <exception>
#include <string>

class UnkownTypeException : public std::exception {
public:
    explicit UnkownTypeException (std::string s) : s(std::move(s)) {}

    UnkownTypeException () = default;

    const char *what () const noexcept override {
        return s.data();
    }

private:
    std::string s;
};

#endif //LUACPP_UNKOWNTYPEEXCEPTION_H

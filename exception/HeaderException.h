#include <utility>

//
// Created by fushenshen on 2020/1/5.
//

#ifndef LUACPP_HEADEREXCEPTION_H
#define LUACPP_HEADEREXCEPTION_H

#include <exception>
#include <string>

class HeaderException : public std::exception {
public:
    explicit HeaderException (std::string s) : s(std::move(s)) {}

    HeaderException () {}

    const char *what () const noexcept override {
        return s.data();
    }

private:
    std::string s;
};

#endif //LUACPP_HEADEREXCEPTION_H

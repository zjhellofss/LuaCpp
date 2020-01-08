//
// Created by fushenshen on 2020/1/7.
//

#include "LuaMath.h"
int64_t iFloorDiv (int64_t a, int64_t b) {
    if ((a > 0 && b > 0) || (a < 0 && b < 0) || a % b == 0) {
        return a / b;
    } else {
        return a / b - 1;
    }
}

double FFloorDiv (double a, double b) {
    return std::floor(a / b);
}

int64_t shiftRight (int64_t a, int64_t n);

int64_t shiftLeft (int64_t a, int64_t n) {
    if (n >= 0) {
        return a << uint64_t(a);
    } else {
        return shiftRight(a, -n);
    }
}

int64_t shiftRight (int64_t a, int64_t n) {
    if (n >= 0) {
        return int64_t(uint64_t(a) >> uint64_t(n));
    } else {
        return shiftLeft(a, -n);
    }
}

int64_t iMod (int64_t a, int64_t b) {
    return a - iFloorDiv(a, b) * b;
}

int64_t Fmod (int64_t a, int64_t b) {
    return static_cast<int64_t>(a - std::floor(a / b) * b);
}

std::pair<int64_t, bool> floatToInteger (double val) {
    auto i = static_cast<int64_t>(val);
    return std::make_pair(i, i == val);
}

std::pair<int64_t, bool> parseInteger (const std::string &str) {
    bool f = true;
    int64_t v;
    try {
        v = std::stoll(str);
    } catch (std::exception &e) {
        f = false;
    }
    return std::make_pair(v, f);
}

std::pair<double, bool> parseDouble (const std::string &str) {
    bool f = true;
    double v;
    try {
        v = std::stod(str);
    } catch (std::exception &e) {
        f = false;
    }
    return std::make_pair(v, f);
}

//
// Created by fushenshen on 2020/1/7.
//

#ifndef LUACPP_MATH_H
#define LUACPP_MATH_H

#include <cmath>
#include <cstdint>
#include <utility>
#include <string>


int64_t iFloorDiv (int64_t a, int64_t b);

double FFloorDiv (double a, double b);

int64_t shiftRight (int64_t a, int64_t n);

int64_t shiftLeft (int64_t a, int64_t n);

int64_t iMod (int64_t a, int64_t b);

int64_t Fmod (int64_t a, int64_t b);

std::pair<int64_t, bool> floatToInteger (double val);

std::pair<int64_t, bool> parseInteger (const std::string &str);

std::pair<double, bool> parseDouble (const std::string &str);


#endif //LUACPP_MATH_H

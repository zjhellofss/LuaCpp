//
// Created by fushenshen on 2020/1/6.
//

#include "instruction.h"

uint32_t getOpcode(uint32_t instruction) {
    return instruction & 0x3F;
}

const std::vector<OpcodeStruct> OpcodeStructVec{
        /*     T  A    B       C     mode         name       action */
        /*     T  A    B       C     mode         name       action */
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "MOVE    ", &LuaVm::move},     // R(A) := R(B)
        OpcodeStruct{0, 1, OpArgK, OpArgN, IABx /* */, "LOADK   ", &LuaVm::loadK},    // R(A) := Kst(Bx)
        OpcodeStruct{0, 1, OpArgN, OpArgN, IABx /* */, "LOADKX  ", &LuaVm::loadKx},   // R(A) := Kst(extra arg)
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */, "LOADBOOL", &LuaVm::loadBool}, // R(A) := (bool)B; if (C) pc++
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABC /* */, "LOADNIL ",
                     &LuaVm::loadNil},  // R(A), R(A+1), ..., R(A+B) := nullptr
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABC /* */, "GETUPVAL", nullptr},      // R(A) := UpValue[B]
        OpcodeStruct{0, 1, OpArgU, OpArgK, IABC /* */, "GETTABUP", nullptr},      // R(A) := UpValue[B][RK(C)]
        OpcodeStruct{0, 1, OpArgR, OpArgK, IABC /* */, "GETTABLE", nullptr},      // R(A) := R(B)[RK(C)]
        OpcodeStruct{0, 0, OpArgK, OpArgK, IABC /* */, "SETTABUP", nullptr},      // UpValue[A][RK(B)] := RK(C)
        OpcodeStruct{0, 0, OpArgU, OpArgN, IABC /* */, "SETUPVAL", nullptr},      // UpValue[B] := R(A)
        OpcodeStruct{0, 0, OpArgK, OpArgK, IABC /* */, "SETTABLE", nullptr},      // R(A)[RK(B)] := RK(C)
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */, "NEWTABLE", nullptr},      // R(A) := {} (size = B,C)
        OpcodeStruct{0, 1, OpArgR, OpArgK, IABC /* */, "SELF    ", nullptr},      // R(A+1) := R(B); R(A) := R(B)[RK(C)]
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "ADD     ", &LuaVm::add},      // R(A) := RK(B) + RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "SUB     ", &LuaVm::sub},      // R(A) := RK(B) - RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "MUL     ", &LuaVm::mul},      // R(A) := RK(B) * RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "MOD     ", &LuaVm::mod},      // R(A) := RK(B) % RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "POW     ", &LuaVm::pow},      // R(A) := RK(B) ^ RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "DIV     ", &LuaVm::div},      // R(A) := RK(B) / RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "IDIV    ", &LuaVm::idiv},     // R(A) := RK(B) // RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "BAND    ", &LuaVm::band},     // R(A) := RK(B) & RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "BOR     ", &LuaVm::bor},      // R(A) := RK(B) | RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "BXOR    ", &LuaVm::bxor},     // R(A) := RK(B) ~ RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "SHL     ", &LuaVm::shl},      // R(A) := RK(B) << RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "SHR     ", &LuaVm::shr},      // R(A) := RK(B) >> RK(C)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "UNM     ", &LuaVm::unm},      // R(A) := -R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "BNOT    ", &LuaVm::bnot},     // R(A) := ~R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "NOT     ", &LuaVm::_not},      // R(A) := not R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "LEN     ", &LuaVm::_len},   // R(A) := length of R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgR, IABC /* */, "CONCAT  ", &LuaVm::concat},   // R(A) := R(B).. ... ..R(C)
        OpcodeStruct{0, 0, OpArgR, OpArgN, IAsBx /**/, "JMP     ",
                     &LuaVm::jmp},      // pc+=sBx; if (A) close all upvalues >= R(A - 1)
        OpcodeStruct{1, 0, OpArgK, OpArgK, IABC /* */, "EQ      ",
                     &LuaVm::eq},       // if ((RK(B) == RK(C)) ~= A) then pc++
        OpcodeStruct{1, 0, OpArgK, OpArgK, IABC /* */, "LT      ",
                     &LuaVm::lt},       // if ((RK(B) <  RK(C)) ~= A) then pc++
        OpcodeStruct{1, 0, OpArgK, OpArgK, IABC /* */, "LE      ",
                     &LuaVm::le},       // if ((RK(B) <= RK(C)) ~= A) then pc++
        OpcodeStruct{1, 0, OpArgN, OpArgU, IABC /* */, "TEST    ", &LuaVm::test},     // if not (R(A) <=> C) then pc++
        OpcodeStruct{1, 1, OpArgR, OpArgU, IABC /* */, "TESTSET ",
                     &LuaVm::testSet},  // if (R(B) <=> C) then R(A) := R(B) else pc++
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */, "CALL    ",
                     nullptr},      // R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */, "TAILCALL", nullptr},      // return R(A)(R(A+1), ... ,R(A+B-1))
        OpcodeStruct{0, 0, OpArgU, OpArgN, IABC /* */, "RETURN  ", nullptr},      // return R(A), ... ,R(A+B-2)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IAsBx /**/, "FORLOOP ",
                     &LuaVm::forLoop},  // R(A)+=R(A+2); if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }
        OpcodeStruct{0, 1, OpArgR, OpArgN, IAsBx /**/, "FORPREP ", &LuaVm::forPrep},  // R(A)-=R(A+2); pc+=sBx
        OpcodeStruct{0, 0, OpArgN, OpArgU, IABC /* */, "TFORCALL",
                     nullptr},      // R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));
        OpcodeStruct{0, 1, OpArgR, OpArgN, IAsBx /**/, "TFORLOOP",
                     nullptr},      // if R(A+1) ~= nullptr then { R(A)=R(A+1); pc += sBx }
        OpcodeStruct{0, 0, OpArgU, OpArgU, IABC /* */, "SETLIST ",
                     nullptr},      // R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABx /* */, "CLOSURE ", nullptr},      // R(A) := closure(KPROTO[Bx])
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABC /* */, "VARARG  ",
                     nullptr},      // R(A), R(A+1), ..., R(A+B-2) = vararg
        OpcodeStruct{0, 0, OpArgU, OpArgU, IAx /*  */, "EXTRAARG", nullptr},      // R(A) := R(B)
};

//获取ABCmode a b c
std::tuple<int, int, int> ABC(uint32_t instruction) {
    int a = instruction >> 6 & 0xFF;
    int c = instruction >> 14 & 0x1FF;
    int b = instruction >> 23 & 0x1FF;
    return std::make_tuple(a, b, c);
}


//获取ABXmode a bx
std::tuple<int, int> ABX(uint32_t instruction) {
    int a = instruction >> 6 & 0xFF;
    int bx = instruction >> 14;
    return std::make_tuple(a, bx);
}

int AX(uint32_t instruction) {
    return instruction >> 6;
}

std::tuple<int, int> AsBx(uint32_t instruction) {
    auto t = ABX(instruction);
    return std::make_tuple(std::get<0>(t), std::get<1>(t) - MAXARG_sBx);
}

std::string opName(uint32_t instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getName();
}

byte opMode(uint32_t instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getOpMode();
}


byte bMode(uint32_t instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getArgBMode();
}

byte cMode(uint32_t instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getArgCMode();
}

void execute(uint32_t instruction, LuaVm *luaVm) {
    auto action = OpcodeStructVec[getOpcode(instruction)].action;
    return (luaVm->*action)(instruction);
}

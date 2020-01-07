//
// Created by fushenshen on 2020/1/6.
//

#include "Opcode.h"

uint32 getOpcode (uint32 instruction) {
    return instruction & 0x3F;
}

std::vector<OpcodeStruct> OpcodeStructVec{
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "MOVE    "}, // R(A) := R(B)
        OpcodeStruct{0, 1, OpArgK, OpArgN, IABx /* */, "LOADK   "}, // R(A) := Kst(Bx)
        OpcodeStruct{0, 1, OpArgN, OpArgN, IABx /* */, "LOADKX  "}, // R(A) := Kst(extra arg)
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */, "LOADBOOL"}, // R(A) := (bool)B; if (C) pc++
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABC /* */, "LOADNIL "}, // R(A), R(A+1), ..., R(A+B) := nil
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABC /* */, "GETUPVAL"}, // R(A) := UpValue[B]
        OpcodeStruct{0, 1, OpArgU, OpArgK, IABC /* */, "GETTABUP"}, // R(A) := UpValue[B][RK(C)]
        OpcodeStruct{0, 1, OpArgR, OpArgK, IABC /* */, "GETTABLE"}, // R(A) := R(B)[RK(C)]
        OpcodeStruct{0, 0, OpArgK, OpArgK, IABC /* */, "SETTABUP"}, // UpValue[A][RK(B)] := RK(C)
        OpcodeStruct{0, 0, OpArgU, OpArgN, IABC /* */, "SETUPVAL"}, // UpValue[B] := R(A)
        OpcodeStruct{0, 0, OpArgK, OpArgK, IABC /* */, "SETTABLE"}, // R(A)[RK(B)] := RK(C)
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */, "NEWTABLE"}, // R(A) := {} (size = B,C)
        OpcodeStruct{0, 1, OpArgR, OpArgK, IABC /* */, "SELF    "}, // R(A+1) := R(B); R(A) := R(B)[RK(C)]
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "ADD     "}, // R(A) := RK(B) + RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "SUB     "}, // R(A) := RK(B) - RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "MUL     "}, // R(A) := RK(B) * RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "MOD     "}, // R(A) := RK(B) % RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "POW     "}, // R(A) := RK(B) ^ RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "DIV     "}, // R(A) := RK(B) / RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "IDIV    "}, // R(A) := RK(B) // RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "BAND    "}, // R(A) := RK(B) & RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "BOR     "}, // R(A) := RK(B) | RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "BXOR    "}, // R(A) := RK(B) ~ RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "SHL     "}, // R(A) := RK(B) << RK(C)
        OpcodeStruct{0, 1, OpArgK, OpArgK, IABC /* */, "SHR     "}, // R(A) := RK(B) >> RK(C)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "UNM     "}, // R(A) := -R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "BNOT    "}, // R(A) := ~R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "NOT     "}, // R(A) := not R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IABC /* */, "LEN     "}, // R(A) := length of R(B)
        OpcodeStruct{0, 1, OpArgR, OpArgR, IABC /* */, "CONCAT  "}, // R(A) := R(B).. ... ..R(C)
        OpcodeStruct{0, 0, OpArgR, OpArgN, IAsBx /**/, "JMP     "}, // pc+=sBx; if (A) close all upvalues >= R(A - 1)
        OpcodeStruct{1, 0, OpArgK, OpArgK, IABC /* */, "EQ      "}, // if ((RK(B) == RK(C)) ~= A) then pc++
        OpcodeStruct{1, 0, OpArgK, OpArgK, IABC /* */, "LT      "}, // if ((RK(B) <  RK(C)) ~= A) then pc++
        OpcodeStruct{1, 0, OpArgK, OpArgK, IABC /* */, "LE      "}, // if ((RK(B) <= RK(C)) ~= A) then pc++
        OpcodeStruct{1, 0, OpArgN, OpArgU, IABC /* */, "TEST    "}, // if not (R(A) <=> C) then pc++
        OpcodeStruct{1, 1, OpArgR, OpArgU, IABC /* */, "TESTSET "}, // if (R(B) <=> C) then R(A) := R(B) else pc++
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */,
                     "CALL    "}, // R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
        OpcodeStruct{0, 1, OpArgU, OpArgU, IABC /* */, "TAILCALL"}, // return R(A)(R(A+1), ... ,R(A+B-1))
        OpcodeStruct{0, 0, OpArgU, OpArgN, IABC /* */, "RETURN  "}, // return R(A), ... ,R(A+B-2)
        OpcodeStruct{0, 1, OpArgR, OpArgN, IAsBx /**/,
                     "FORLOOP "}, // R(A)+=R(A+2); if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }
        OpcodeStruct{0, 1, OpArgR, OpArgN, IAsBx /**/, "FORPREP "}, // R(A)-=R(A+2); pc+=sBx
        OpcodeStruct{0, 0, OpArgN, OpArgU, IABC /* */, "TFORCALL"}, // R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));
        OpcodeStruct{0, 1, OpArgR, OpArgN, IAsBx /**/, "TFORLOOP"}, // if R(A+1) ~= nil then { R(A)=R(A+1); pc += sBx }
        OpcodeStruct{0, 0, OpArgU, OpArgU, IABC /* */, "SETLIST "}, // R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABx /* */, "CLOSURE "}, // R(A) := closure(KPROTO[Bx])
        OpcodeStruct{0, 1, OpArgU, OpArgN, IABC /* */, "VARARG  "}, // R(A), R(A+1), ..., R(A+B-2) = vararg
        OpcodeStruct{0, 0, OpArgU, OpArgU, IAx /*  */, "EXTRAARG"},
};


//获取ABCmode a b c
std::tuple<int, int, int> ABC (uint32 instruction) {
    int a = instruction >> 6 & 0xFF;
    int c = instruction >> 14 & 0x1FF;
    int b = instruction >> 23 & 0x1FF;
    return std::make_tuple(a, b, c);
}


//获取ABXmode a bx
std::tuple<int, int> ABX (uint32 instruction) {
    int a = instruction >> 6 & 0xFF;
    int bx = instruction >> 14;
    return std::make_tuple(a, bx);
}

int AX (uint32 instruction) {
    return instruction >> 6;
}

std::tuple<int, int> AsBx (uint32 instruction) {
    auto t = ABX(instruction);
    return std::make_tuple(std::get<0>(t), std::get<1>(t) - MAXARG_sBx);
}

std::string opName (uint32 instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getName();
}

byte opMode (uint32 instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getOpMode();
}


byte bMode (uint32 instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getArgBMode();
}

byte cMode (uint32 instruction) {
    return OpcodeStructVec[getOpcode(instruction)].getArgCMode();
}


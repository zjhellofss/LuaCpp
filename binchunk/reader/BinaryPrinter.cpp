#include "BinaryReader.h"
#include "BinaryPrinter.h"
#include "../BinaryChunk.h"
#include "../instruction.h"

#include <cstdio>

void printHeader (ProtoType *f) {
    std::string funcType("main");
    std::string varagFlag;
    if (f->isVarag) {
        varagFlag = "+";
    }

    printf("\n%s <%s:%d,%d> (%ld instructions)\n",
           funcType.data(), f->source.data(), f->lineDefined, f->lastLineDefined, f->code.size());

    printf("%d%s params, %d slots, %ld upvalues, ",
           f->numParams, varagFlag.data(), f->maxStackSize, f->upvalues.size());


    printf("%ld locals, %ld constants, %ld functions\n",
           f->locVar.size(), f->constants.size(), f->protos.size());
}


std::string constantToString (Interface *inst) {
    int t = inst->type;
    if (t == TAG_NIL) {
        return "";
    } else if (t == TAG_BOOLEAN) {//boolean
        if (*((bool *) (inst->val))) {
            return "true";
        } else {
            return "false";
        }
    } else if (t == TAG_INTEGER) {//integer
        return std::to_string(*((int *) inst->val));
    } else if (t == TAG_LONG_STR || t == TAG_SHORT_STR) {
        return std::string(*(std::string *) inst->val);
    } else {
        throw std::logic_error("Unkown type exception");
    }
}

std::string upValName (ProtoType *f, int idx) {
    if (!f->upvalueNames.empty()) {
        return f->upvalueNames[idx];
    } else {
        return "-";
    }
}

void printDetial (ProtoType *f) {
    size_t s1 = f->constants.size();
    printf("constants (%ld):\n", s1);
    for (auto i = 0; i < s1; ++i) {
        Interface *it = f->constants[i];
        printf("\t%d\t%s\n", i + 1, constantToString(it).data());
    }

    size_t s2 = f->locVar.size();
    printf("locals (%ld):\n", s2);
    for (auto i = 0; i < s2; ++i) {
        LocVal locVal = f->locVar[i];
        printf("\t%d\t%s\t%d\t%d\n",
               i, locVal.varName.data(), locVal.startPC + 1, locVal.endPC + 1);
    }

    size_t s3 = f->upvalues.size();
    printf("upvalues (%ld):\n", s3);
    for (auto i = 0; i < s3; ++i) {
        printf("\t%d\t%s\t%d\t%d\n",
               i, upValName(f, i).data(), f->upvalues[i].instack, f->upvalues[i].idx);
    }

}


std::shared_ptr<ProtoType> undump (const std::string &path) {
    BinaryReader reader(path);
    reader.checkReader();
    reader.readByte();
    return reader.readProto("");
}


void printBinary (const std::string &path) {
    auto f = undump(path);
    list(f.get());
}

void printOperands (uint32_t instruction) {
    auto t = opMode(instruction);
    if (t == IABC) {
        int a, b, c;
        std::tie(a, b, c) = ABC(instruction);
        printf("%d", a);
        if (bMode(instruction) != OpArgN) {
            if (b > 0xFF) {
                printf(" %d", -1 - b & 0xFF);
            } else {
                printf(" %d", b);
            }
        }
        if (cMode(instruction) != OpArgN) {
            if (c > 0xFF) {
                printf(" %d", -1 - (c & 0xFF));
            } else {
                printf(" %d", c);
            }
        }
    } else if (t == IAsBx) {
        int a, sbx;
        std::tie(a, sbx) = AsBx(instruction);
        printf("%d %d", a, sbx);
    } else if (t == IAx) {
        int ax = AX(instruction);
        printf("%d", -1 - ax);
    } else if (t == IABx) {
        int a, bx;
        std::tie(a, bx) = ABX(instruction);
        printf("%d", a);
        if (bMode(instruction) == OpArgK) {
            printf(" %d", -1 - bx);
        } else if (bMode(instruction) == OpArgU) {
            printf(" %d", bx);
        }
    }
}


void printCode (ProtoType *f) {
    auto codes = f->code;
    size_t size = codes.size();
    for (auto pc = 0; pc < size; ++pc) {
        std::string line = "-";
        if (!f->lineInfo.empty()) {
            int v = f->lineInfo[pc];
            line = std::to_string(v);
        }
        uint32_t inst = codes[pc];//获取对应的指令
        printf("\t%d\t[%s]\t%s \t", pc + 1, line.data(), opName(inst).data());
        printOperands(inst);
        printf("\n");
    }
}


void list (ProtoType *f) {
    printHeader(f);
    printCode(f);
    printDetial(f);
    for (const auto &e:f->protos) {
        list(e.get());
    }
}



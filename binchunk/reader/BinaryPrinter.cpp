#include "BinaryReader.h"
#include "BinaryPrinter.h"
#include "../BinaryChunk.h"
#include "../../exception/UnkownTypeException.h"
#include <cstdio>

static void printHeader (ProtoType *f) {
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

static void printCode (ProtoType *f) {
    auto codes = f->code;
    size_t size = codes.size();
    for (auto pc = 0; pc < size; ++pc) {
        std::string line = "-";
        if (!f->lineInfo.empty()) {
            int v = f->lineInfo[pc];
            line = std::to_string(v);
        }
        printf("\t%d\t[%s]\t0x%08X\n", pc + 1, line.data(), codes[pc]);
    }
}


static std::string constantToString (const interface &inst) {
    int t = inst.type;
    if (t == TAG_NIL) {
        return "";
    } else if (t == TAG_BOOLEAN) {//boolean
        if (inst.boolean) {
            return "true";
        } else {
            return "false";
        }
    } else if (t == TAG_INTEGER) {//integer
        return std::to_string(inst.integer);
    } else if (t == TAG_LONG_STR || t == TAG_SHORT_STR) {
        return inst.valStr;
    } else {
        throw UnkownTypeException("Unkown type exception");
    }
}

static std::string upValName (ProtoType *f, int idx) {
    if (!f->upvalueNames.empty()) {
        return f->upvalueNames[idx];
    } else {
        return "-";
    }
}

static void printDetial (ProtoType *f) {
    size_t s1 = f->constants.size();
    printf("constants (%ld):\n", s1);
    for (auto i = 0; i < s1; ++i) {
        interface it = f->constants[i];
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


static void list (ProtoType *f) {
    printHeader(f);
    printCode(f);
    printDetial(f);
    for (const auto &e:f->protos) {
        list(e.get());
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


//"/Users/fss/CLionProjects/Luacpp/test/luac.out"

#include "./binchunk/reader/BinaryReader.h"
#include "./binchunk/reader/BinaryPrinter.h"
#include "./luaStruct/LuaVm.h"
#include "./binchunk/Instruction.h"

int main () {
    auto p = undump("/Users/fss/CLionProjects/LuaCpp/LuaCpp/test/luac.out");
    ProtoType *protoType = p.get();
    auto *luaVm = new LuaVm(protoType);
    int maxSize = protoType->maxStackSize;
    luaVm->setTop(maxSize);
    while (true) {
        int pc = luaVm->Pc();
        uint32_t inst = luaVm->fetch();
        if (Opcode(inst) != OP_RETURN) {
            execute(inst, luaVm);
            printf("[%2d] %s ", pc + 1, opName(inst).c_str());
            luaVm->printStack();
        } else {
            break;
        }
    }
    delete luaVm;
    return 0;
}




#include "BinaryReader.h"
#include "../BinaryChunk.h"

byte BinaryReader::readByte () {
    byte b = bytes[pos];
    pos += 1;
    return b;
}

std::vector<byte> BinaryReader::readBytes (const int &len) {
    std::vector<byte> data(bytes + pos, bytes + pos + len);
    pos += len;
    return data;
}


uint32 BinaryReader::readUint32 () {
    std::vector<byte> vec = readBytes(4);
    auto e = vec.begin();
    for (; e != vec.end(); ++e) {
        if (*e != '\0') {
            break;
        }
    }
    if (e != vec.begin()) {
        vec.erase(vec.begin(), e);
    }
    byte *data = vec.data();
    auto i = reinterpret_cast<uint32 *>(data);
    return *i;
}


uint64_t BinaryReader::readUint64 () {
    std::vector<byte> vec = readBytes(8);
    auto e = vec.begin();
    for (; e != vec.end(); ++e) {
        if (*e != '\0') {
            break;
        }
    }
    if (e != vec.begin()) {
        vec.erase(vec.begin(), e);
    }
    byte *data = vec.data();
    auto i = reinterpret_cast<uint64_t *>(data);
    return *i;
}

int64_t BinaryReader::readLuaInteger () {
    auto uint = readUint64();
    int64_t i = uint;
    return i;
}

double BinaryReader::readLuaNumber () {
    std::vector<byte> vec = readBytes(8);
    auto e = vec.begin();
    for (; e != vec.end(); ++e) {
        if (*e != '\0') {
            break;
        }
    }
    if (e != vec.begin()) {
        vec.erase(vec.begin(), e);
    }
    byte *data = vec.data();
    double a;
    std::copy(data, data + sizeof(double), reinterpret_cast<char *>(&a));
    return a;
}

std::string BinaryReader::readString () {
    size_t size = this->readByte();
    if (size == 0) {
        return "";
    }
    if (size == 0xFF) {
        size = this->readUint64();
    }
    auto bytes = this->readBytes(static_cast<int>(size - 1)).data();
    return std::string(reinterpret_cast<char *const>(bytes));
}

void BinaryReader::checkReader () {
    std::string signature = std::string(reinterpret_cast<char *const>(this->readBytes(4).data()));
    if (signature != LUA_SIGNATURE) {
        // luac文件签名不一致的情况
        throw HeaderException("signature error");
    }
    byte version = this->readByte();
    if (version != LUAC_VERSION) {
        // luac版本号不一致
        throw HeaderException("version error");
    }
    byte format = this->readByte();
    if (format != LUAC_FORMAT) {
        throw HeaderException("format error");
    }
    std::string ldata = std::string(reinterpret_cast<char *const>( this->readBytes(6).data()));
    if (ldata != LUAC_DATA) {
        // lua中固定的字符串不一致
        throw HeaderException("ldata error");
    }
    byte cintSize = this->readByte();
    if (cintSize != CINT_SIZE) {
        // lua中cint值的大小
        throw HeaderException("cint error");
    }
    byte cstSize = this->readByte();
    if (cstSize != CSIZET_SIZE) {
        throw HeaderException("csize error");
    }
    byte instSize = this->readByte();
    if (instSize != INSTRUCTION_SIZE) {
        throw HeaderException("instSize error");
    }
    byte integerSize = this->readByte();
    if (integerSize != LUA_INTEGER_SIZE) {
        throw HeaderException("integer error");
    }
    byte numberSize = this->readByte();
    if (numberSize != LUA_NUMBER_SIZE) {
        throw HeaderException("number error");
    }
    int64_t luacInt = this->readLuaInteger();
    if (luacInt != LUAC_INT) {
        throw HeaderException("luacInt error");
    }
    double luacNum = this->readLuaNumber();
    if (luacNum - LUAC_NUM > 1e-6) {
        throw HeaderException("luacNum error");
    }

}

std::shared_ptr<ProtoType> BinaryReader::readProto (const std::string &parentSource) {
    std::string source = this->readString();
    if (source.empty()) {
        source = parentSource;
    }
    auto p = std::shared_ptr<ProtoType>(
            new ProtoType{
                    source,
                    this->readUint32(),//起始行号
                    this->readUint32(),//终结行号
                    this->readByte(),//numParams
                    this->readByte(),//isVarag
                    this->readByte(),//maxStackSize 寄存器的数量
                    this->readCode(),//
                    this->readConstants(),
                    this->readUpValues(),
                    //子函数表
                    this->readProtos(parentSource),
                    this->readLineInfo(),
                    this->readLocVals(),
                    this->readUpValueNames()
            }
    );
    return p;
}

//读取指令表
std::vector<uint32> BinaryReader::readCode () {
    std::vector<uint32> codes;
    size_t size = this->readUint32();
    codes.reserve(size);
    for (int i = 0; i < size; ++i) {
        codes.push_back(this->readUint32());
    }
    return codes;
}

//读取常量
Interface *BinaryReader::readConstant () {
    byte b = this->readByte();
    Interface *f = new Interface;
    f->type = b;
    if (b == TAG_NIL) {
    } else if (b == TAG_BOOLEAN) {
        byte v = this->readByte();
        f->val = new bool((v != 0));
    } else if (b == TAG_NUMBER) {
        f->val = new double(this->readLuaNumber());
    } else if (b == TAG_LONG_STR || b == TAG_SHORT_STR) {
        std::string str = this->readString();
        f->val = new std::string(str);
    } else if (b == TAG_INTEGER) {
        f->val = new int(this->readLuaInteger());
    }
    return f;
}

std::vector<Interface *> BinaryReader::readConstants () {
    size_t size = this->readUint32();
    std::vector<Interface *> interfaces;
    interfaces.reserve(size);
    for (int i = 0; i < size; ++i) {
        interfaces.push_back(this->readConstant());
    }
    return interfaces;
}

std::vector<Upvalue> BinaryReader::readUpValues () {
    std::vector<Upvalue> upvalues;
    int size = this->readUint32();
    byte instack;
    byte idx;
    for (int i = 0; i < size; ++i) {
        instack = this->readByte();
        idx = this->readByte();
        upvalues.emplace_back(Upvalue(instack, idx));
    }
    return upvalues;
}

std::vector<uint32> BinaryReader::readLineInfo () {
    std::vector<uint32> lineInfos;
    size_t size = this->readUint32();
    lineInfos.reserve(size);
    for (int i = 0; i < size; ++i) {
        lineInfos.push_back(this->readUint32());
    }
    return lineInfos;
}

std::vector<LocVal> BinaryReader::readLocVals () {
    std::vector<LocVal> locVals;
    int size = this->readUint32();
    std::string varName;
    uint32 startPc;
    uint32 endPc;
    for (int i = 0; i < size; ++i) {
        varName = this->readString();
        startPc = this->readUint32();
        endPc = this->readUint32();
        locVals.emplace_back(varName, startPc, endPc);
    }
    return locVals;
}

//读取子函数
std::vector<std::shared_ptr<ProtoType>> BinaryReader::readProtos (const std::string &parentSource) {
    std::vector<std::shared_ptr<ProtoType>> protos;
    size_t size = this->readUint32();
    protos.reserve(size);
    for (int i = 0; i < size; ++i) {
        protos.push_back(this->readProto(parentSource));
    }
    return protos;
}


std::vector<std::string> BinaryReader::readUpValueNames () {
    std::vector<std::string> upValueNames;
    size_t size = this->readUint32();
    upValueNames.reserve(size);
    for (int i = 0; i < size; ++i) {
        upValueNames.push_back(this->readString());
    }
    return upValueNames;
}



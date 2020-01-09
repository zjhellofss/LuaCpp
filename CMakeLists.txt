cmake_minimum_required(VERSION 3.12)
project(LuaCpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(LuaCpp  binchunk/BinaryChunk.h binchunk/BinaryChunk.h binchunk/reader/BinaryReader.cpp binchunk/reader/BinaryReader.h  binchunk/reader/BinaryPrinter.cpp binchunk/reader/BinaryPrinter.h binchunk/Instruction.h binchunk/Instruction.cpp luaState/LuaState.h luaState/LuaState.cpp luaStruct/LuaMath.cpp luaStruct/LuaMath.h luaStruct/LuaValue.cpp luaStruct/LuaValue.h luaStruct/LuaVm.cpp luaStruct/LuaVm.h main.cpp)
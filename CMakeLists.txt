cmake_minimum_required(VERSION 3.12)
project(LuaCpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(LuaCpp  binchunk/BinaryChunk.h binchunk/BinaryChunk.h binchunk/reader/BinaryReader.cpp binchunk/reader/BinaryReader.h  binchunk/reader/BinaryPrinter.cpp binchunk/reader/BinaryPrinter.h binchunk/Instruction.h binchunk/Instruction.cpp api/LuaState.h api/LuaState.cpp api/LuaMath.cpp api/LuaMath.h api/LuaValue.cpp api/LuaValue.h api/LuaVm.cpp api/LuaVm.h main.cpp)
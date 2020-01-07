cmake_minimum_required(VERSION 3.12)
project(LuaCpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(LuaCpp  binchunk/BinaryChunk.h binchunk/BinaryChunk.h binchunk/reader/BinaryReader.cpp binchunk/reader/BinaryReader.h test/test.cpp binchunk/reader/BinaryPrinter.cpp binchunk/reader/BinaryPrinter.h  binchunk/Opcode.h binchunk/Opcode.cpp api/LuaState.h api/LuaState.cpp)
cmake_minimum_required(VERSION 3.10)
project(jupiter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

include_directories( ./include )

add_executable(
  jupiter
  src/main.cpp
  src/vm/World.cpp
  src/vm/VM.cpp
  src/vm/Stack.cpp
  src/vm/ObjectSerializer.cpp
  src/vm/Frame.cpp
  src/vm/ConstantsTable.cpp
  src/utils/files.cpp
  src/primitives/functions.cpp
  src/primitives/primitives.cpp
  src/objects/Array.cpp
  src/objects/CompiledMethod.cpp
  src/objects/Map.cpp
  src/objects/Method.cpp
  src/objects/NativeMethod.cpp
  src/objects/Number.cpp
  src/objects/Object.cpp
  src/objects/String.cpp
  src/objects/UserData.cpp
  src/memory/GC.cpp
  src/memory/memory.cpp
  src/extensions/NativeLibraries.cpp
  src/compiler/ASTNode.cpp
  src/compiler/Compiler.cpp
  src/compiler/Lexer.cpp
  src/compiler/Parser.cpp
  src/compiler/SymbolTable.cpp
  src/compiler/Token.cpp)

target_link_libraries (
  jupiter
  mpdec
  dl)

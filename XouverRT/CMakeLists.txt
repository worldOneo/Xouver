cmake_minimum_required (VERSION 3.0)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_library (xouverrt STATIC "runtime/runtime.cpp" "runtime/runtime.h" "runtime/runtimeprocess.cpp" "runtime/runtimeprocess.h" "class/xclass.cpp" "class/xclass.h" "common.cpp" "common.h"   "class/native/nativeclasses.h"  "object/object.h" "opcodes.h" "class/classmanager.cpp" "class/classmanager.h" "value/valuetype.h" "value/xvalue.h" "memory/memory.h" "memory/memory.cpp" "mapping/functionmap.cpp" "mapping/functionmap.h" "mapping/functionmap.cpp" "mapping/functionmap.h" "mapping/classmap.h" "mapping/classmap.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xouverrt PROPERTY CXX_STANDARD 20)
endif()
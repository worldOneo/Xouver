cmake_minimum_required (VERSION 3.0)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_library (xouverrt STATIC "runtime/runtime.cpp" "runtime/runtime.h" "runtime/runtimeprocess.cpp" "runtime/runtimeprocess.h" "class/xclass.cpp" "class/xclass.h" "common.cpp" "common.h"   "class/native/nativeclasses.h" "object/object.cpp" "object/object.h" "memory/memorymanager.cpp" "memory/memorymanager.h" "instructions.h" "class/classmanager.cpp" "class/classmanager.h" "object/object.cpp"  "value/valuetype.h" "value/xvalue.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xouverrt PROPERTY CXX_STANDARD 20)
endif()
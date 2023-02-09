#include "functionmap.h"

#include <memory/memory.h>

functioninfo& function_map::getFunction(int index) {
	return functions[index];
}

void function_map::putFunction(functioninfo info) {
	functions.push_back(info);
}
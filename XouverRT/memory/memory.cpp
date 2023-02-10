#include "memory.h"

#include <XNI.h>

#include <runtime/runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

static const std::size_t slabSize = 1 << 19;
static const std::size_t youngGenerationSize = 16;

memorymanager::memorymanager(void* rt) {
	this->rt = rt;
}

xobject memorymanager::allocate() {
	auto& memory = getYoung();
	if (memory.size() == youngGenerationSize) {
		auto rt = (runtime*)this->rt;
		rt->fullStack();
		/* GC */
	}
	memory.push_back({});	 // TODO: Object creation
	return (xobject)&memory[memory.size() - 1];
}

memorymanager::~memorymanager() {}

memorymanager::Memory& memorymanager::getYoung() {
	return isYoungA ? youngA : youngB;
}
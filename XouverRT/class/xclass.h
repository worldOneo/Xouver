#ifndef XCLASS_H
#define XCLASS_H

#include <XNI.h>
#include <string>
#include <vector>

#include "value/xvalue.h"

class xclass {
public:
	void* rt;

	int classOffset;
	std::string fullName;

	std::vector<xvalue> pool;
	int poolSize;

	int functionsSize;
	int* functions;

	int funcsOffset;

	int scopeSize;
};

#endif
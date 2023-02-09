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
	char* fullName;

	xvalue** pool;
	int poolSize;

	int functionsSize;
	int* functions;

	int funcsOffset;

	int scopeSize;
};

#endif
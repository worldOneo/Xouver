#ifndef XCLASS_H
#define XCLASS_H

#include <XNI.h>
#include <string>
#include <vector>

#include "value/xvalue.h"

class xclass {
private:
	void* rt;
	xvalue** pool;
	int poolSize;
	int scopeSize;

public:
	xclass(void* rt, xvalue** pool, int poolSize, int scopeSize);
	xvalue* getConstant(int index);
	int getScopeSize();
};

#endif
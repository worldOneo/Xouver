#ifndef XCLASS_H
#define XCLASS_H

#include <XNI.h>
#include <string>
#include <vector>

#include <memory/memorymanager.h>

class xclass {
private:
	void* rt;
	ptr** pool;
	int poolSize;
	int scopeSize;

public:
	xclass(void* rt, ptr** pool, int poolSize, int scopeSize);
	ptr* getConstant(int index);
};

#endif
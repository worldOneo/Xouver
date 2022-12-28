#include "xclass.h"

#include <string>
#include <runtime/runtime.h>

#include <iostream>

xclass::xclass(void* rt, xvalue** pool, int poolSize, int scopeSize) {
	this->rt = rt;
	this->pool = (xvalue**)malloc(sizeof(xvalue*) * poolSize);
	if (this->pool == nullptr) throw std::exception("Could not allocate class pool");
	memcpy(this->pool, pool, sizeof(xvalue*) * poolSize);
	this->poolSize = poolSize;
	this->scopeSize = scopeSize;
}

xvalue* xclass::getConstant(int index) {
	if (index >= poolSize) {
		((runtime*)rt)->setError(XRT_Error::POOL_INDEX_OUT_OF_RANGE);
		return nullptr;
	}
	else {
		return pool[index];
	}
}

int xclass::getScopeSize() {
	return this->scopeSize;
}
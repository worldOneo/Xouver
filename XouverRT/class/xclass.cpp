#include "xclass.h"

#include <string>
#include <runtime/runtime.h>


xclass::xclass(void* rt, ptr** pool, int poolSize, int scopeSize) {
	this->rt = rt;
	this->pool = pool;
	this->poolSize = poolSize;
	this->scopeSize = scopeSize;
}

ptr* xclass::getConstant(int index) {
	if (index >= poolSize) {
		((runtime*)rt)->setError(XRT_Error::POOL_INDEX_OUT_OF_RANGE);
		return nullptr;
	}
	else {
		return pool[index];
	}
}
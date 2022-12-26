#include "object.h"

#include <class/xclass.h>

object::object(xclass* classObj, int scopeSize) {
	this->classObj = classObj;
	this->scopeSize = scopeSize;

	this->obj = new ptr*[scopeSize];
}

ptr* object::get(int idx) {
	return obj[idx];
}

object::~object() {
	delete this->obj;
}
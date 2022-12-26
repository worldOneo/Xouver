#include "memorymanager.h"

#include <XNI.h>
#include <cstdlib>

#include <runtime/runtime.h>


ptr::ptr() {
	this->p = nullptr;
}

ptr::ptr(xvalue v) {
	memorymanager::allocate(this, sizeof(v));
	*this->p = v;
}

ptr::ptr(xvalue* v) {
	memorymanager::allocate(this, sizeof(*v));
	*this->p = *v;
}

bool ptr::operator==(xvalue& value) {
	return *p == value;
}

bool ptr::operator==(ptr p) {
	return p == *this;
}

bool ptr::operator==(ptr* p) {
	return p == this;
}

void ptr::operator=(ptr value) {
	xvalue* _p = this->p;
	this->p = value.p;

	value.p = _p;
	value.refCount--;
	this->refCount++;
}

void ptr::operator=(xvalue& value) {
	free(this->p);
	memorymanager::allocate(this, sizeof(value));
	*this->p = value;
}

void ptr::operator=(xvalue* value) {
	free(this->p);
	*this->p = *value;
}

xvalue& ptr::operator* () {
	return *p;
}

ptr::~ptr() {
	free(this->p);
}

int memorymanager::ptrsCount = 0;
ptr** memorymanager::ptrs = (ptr**)malloc(ptrsCount * sizeof(ptr*));

void memorymanager::initialize() {
}

ptr* memorymanager::allocate(ptr* p, size_t size) {
	xvalue* _p = (xvalue*)malloc(size);

	if (_p == nullptr) return nullptr;
	p->p = _p;
	p->refCount++;

	put(p);
	return p;
}

void memorymanager::put(ptr* p) {
	ptrsCount++;
	ptrs = (ptr**)realloc(ptrs, sizeof(ptr*) * ptrsCount);
	ptrs[ptrsCount - 1] = p;
}


bool memorymanager::reallocate(ptr* p, size_t nsize) {
	xvalue* _p = p->p;
	_p = (xvalue*) realloc(_p, nsize);

	if (_p == nullptr) return false;

	p->p = _p;
	return true;
}

void memorymanager::deallocate(ptr* p) {
	bool found = false;

	for (int i = 0; i < ptrsCount; i++) {
		if (found) {
			ptrs[i - 1] = ptrs[i];
		}
		if (ptrs[i] == p) {
			delete p;
			found = true;
		}
	}

	ptrsCount--;
	ptrs = (ptr**)realloc(ptrs, sizeof(ptr*) * ptrsCount);
}

void memorymanager::clear() {
	for (int i = 0; i < ptrsCount; i++) {
		delete ptrs[i];
	}
}
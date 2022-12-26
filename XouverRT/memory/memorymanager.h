#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <XNI.h>
#include <cstdio>
#include <vector>

#include <value/xvalue.h>


struct ptr {
private:
	xvalue* p;
	int refCount;
public:
	ptr();
	ptr(xvalue v);
	ptr(xvalue* v);

	bool operator == (xvalue& value);
	bool operator == (ptr p);
	bool operator == (ptr* p);
	void operator = (ptr value);
	void operator = (xvalue& value);
	void operator = (xvalue* value);
	xvalue& operator* ();

	~ptr();

	friend class memorymanager;
};


static class memorymanager {
private:
	static int ptrsCount;
	static ptr** ptrs;
public:
	static void initialize();

	static ptr* allocate(ptr* p, size_t size);
	static void put(ptr* ptr);
	static bool reallocate(ptr* p, size_t nsize);
	static void deallocate(ptr* p);

	static void cleanup();
	static void clear();
} memorymanager;

#endif
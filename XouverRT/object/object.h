#ifndef OBJECT_H
#define OBJECT_H

#include <XNI.h>
#include <string>
#include <class/xclass.h>
#include <memory/memorymanager.h>

class object : _xobject {
private:
	int scopeSize;
	xclass* classObj;
	ptr** obj;
public:
	object(xclass* classObj, int scopeSize);
	ptr* get(int idx);
	~object();
};

#endif
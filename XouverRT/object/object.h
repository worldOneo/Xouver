#ifndef OBJECT_H
#define OBJECT_H

#include <XNI.h>
#include <class/xclass.h>
#include <string>

struct object : _xobject {
	xclass* classObj;
	xvalue* obj;
};

#endif
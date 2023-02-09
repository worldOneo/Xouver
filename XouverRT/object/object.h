#ifndef OBJECT_H
#define OBJECT_H

#include <XNI.h>
#include <string>
#include <class/xclass.h>

struct object : _xobject {
	xclass* classObj;
	xvalue** obj;
};

#endif
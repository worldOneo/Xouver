#ifndef NATIVE_CLASSES_H
#define NATIVE_CLASSES_H

#include <iostream>

#include <runtime/runtime.h>
#include <memory/memorymanager.h>

class nativeObject {
public:
	static void equals(void* rt, xfunc_data* data) {
		//std::cout << getStackTop(rt).i << "\n";
	}
};

class nativeInt : public nativeObject {
public:
	static void toString(void* rt, xfunc_data* data) {
		std::cout << data->params[0].i << std::endl;
	}
};

class nativeFloat : public nativeObject {

};

class nativeString : public nativeObject {

};

class nativeArray : public nativeObject {

};

#endif
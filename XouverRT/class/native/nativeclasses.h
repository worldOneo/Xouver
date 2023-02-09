#ifndef NATIVE_CLASSES_H
#define NATIVE_CLASSES_H

#include <iostream>

class nativeObject {
public:
	static void equals(void* rt) {
		std::cout << "false" << "\n";
	}
};

class nativeInt : public nativeObject {
public:
	static void toString(void* rt) {
		std::cout << "true" << std::endl;
	}
};

class nativeFloat : public nativeObject {

};

class nativeString : public nativeObject {

};

class nativeArray : public nativeObject {

};

#endif
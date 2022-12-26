#include "common.h"


#define XRT
#include <XNI.h>
#include <exception>
#include <iostream>
#include <string>

#include <runtime/runtime.h>
#include <instructions.h>

#include <class/native/nativeclasses.h>
#include <class/xclass.h>
#include <class/classmanager.h>
#include <memory/memorymanager.h>

#define VERSION_MAJ 1
#define VERSION_MIN 0
#define XNI_VERIFY_NUM 0x2008

XNI_Error currentError = XNI_Error::NO_XNI_ERROR;
runtime* rt;

void* createRuntime() {
	memorymanager::initialize();

	runtime* rt = new runtime();
	ptr* pool[1];
	pool[0] = new ptr(5);

	xclass* c = (xclass*)malloc(sizeof(xclass));
	*c = xclass(rt, pool, 1, 0);

	unsigned char* instructions = new unsigned char[] {
		INST_CLOAD, 0, INST_OUT, 8
	};

	registerFunction(rt, "int:ToString()#void", *nativeInt::toString);
	registerFunction(rt, "object:Equals()#void", *nativeObject::equals);
	registerFunction(rt, "xouver:Main()#void", instructions);
	
	registerClass(rt, "Xouver.Test.TestClass", c);
	return rt;
}

runtime* _convertRT(void* rt) {
	try {
		return ((runtime*)rt);
	}
	catch (std::exception e) {
		currentError = XNI_Error::POINTER_TO_NO_RT;
		return nullptr;
	}
}
#define convertRT(_rt) if ((rt = _convertRT(_rt)) == nullptr) XRT_Error::INTERNAL_XNI_FAILURE;

bool xniVerify(char c) {
	return (c == XNI_VERIFY_NUM);
}

int getXMajorVersion() {
	return VERSION_MAJ;
}

int getXMinorVersion() {
	return VERSION_MIN;
}


void setXniError(XNI_Error e) {
	currentError = e;
}

bool xniHasError() {
	return (currentError != XNI_Error::NO_XNI_ERROR);
}

bool rtHasError(void* _rt) {
	convertRT(_rt);

	return (rt->getError() != XRT_Error::NO_XRT_ERROR);
}

XNI_Error getXniError() {
	return currentError;
}

XRT_Error getXrtError(void* _rt) {
	convertRT(_rt);

	return rt->getError();
}

void registerFunction(void* _rt, const char* signature, void (*fn)(void*, xfunc_data*)) {
	convertRT(_rt);

	rt->putNativeFunction(std::string(signature), fn);
}

void registerFunction(void* _rt, const char* signature, unsigned char* instructions) {
	convertRT(_rt);

	rt->putFunction(signature, instructions);
}

void callFunction(void* _rt, const char* signature, unsigned int argCount) {
	convertRT(_rt);

	rt->callFunction(signature, {}, argCount);
}

void runRuntime(void* _rt, const char* mainClass, const char* mainFuncSignature) {
	convertRT(_rt);

	xclass* c = rt->getClassManager()->getClass(mainClass);
	rt->run(c, mainFuncSignature);
	memorymanager::clear();
}

void haltCurrentProcess(void* _rt) {
	convertRT(_rt);

	rt->haltProcess();
}

void haltRuntime(void* _rt) {
	convertRT(_rt);

	while (rt->haltProcess()) { rt->switchProcess(); }
}

void throwError(void* _rt, const char* msg) {
	convertRT(_rt);

	rt->throwError(msg);
}

int getExpceptionMessageSize(void* _rt) {
	convertRT(_rt);

	return rt->getException().size();
}

void getExceptionMessage(void* _rt, char** outMsg) {
	convertRT(_rt);

	for (int i = 0; i < rt->getException().size(); i++) {
		(*outMsg)[i] = rt->getException()[i];
	}
}

void registerClass(void* _rt, const char* signature, xclass* c) {
	convertRT(_rt);

	rt->getClassManager()->putClass(signature, c);
}

/*const xvalues& getStackTop(void* _rt) {
	convertRT(_rt);

	return rt->getStackTop();
}*/
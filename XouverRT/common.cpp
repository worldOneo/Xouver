#include "common.h"


#define XRT
#include <XNI.h>
#include <exception>
#include <iostream>
#include <string>

#include <runtime/runtime.h>
#include <opcodes.h>

#include <class/native/nativeclasses.h>
#include <class/xclass.h>
#include <class/classmanager.h>

#include <memory/memory.h>

#define VERSION_MAJ 1
#define VERSION_MIN 0
#define XNI_VERIFY_NUM 0x2008

XNI_Error currentError = XNI_Error::NO_XNI_ERROR;
runtime* rt;

void* createRuntime() {
	initMem();
	runtime* rt = new runtime();

	xvalue* pool[2];
	pool[0] = (xvalue*)allocate(sizeof(xvalue));

	pool[0]->type = valuetype::INT;
	pool[0]->value.i = 5;

	pool[1] = (xvalue*)allocate(sizeof(xvalue));
	pool[1]->type = valuetype::INT;
	pool[1]->value.i = 5;

	xclass* c = (xclass*)allocate(sizeof(xclass));
	*c = xclass(rt, pool, 2, 0);

	int* instructions = new int[] {
		OP_CLOAD, 0,
		OP_CLOAD, 1,
		OP_IFEQ,
		OP_JIF, 11,
		OP_CALL, 1,
		OP_JMP, 13,
		OP_CALL, 2,
		8
	};

	registerFunction(rt, "int:ToString()#void", *nativeInt::toString);
	registerFunction(rt, "object:Equals()#void", *nativeObject::equals);
	registerFunction(rt, "xouver:Main()#void", instructions);

	rt->mapFunction(0, "xouver:Main()#void");
	rt->mapFunction(1, "object:Equals()#void");
	rt->mapFunction(2, "int:ToString()#void");

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

void registerFunction(void* _rt, const char* signature, void (*fn)(void*)) {
	convertRT(_rt);

	rt->putNativeFunction(std::string(signature), fn);
}

void registerFunction(void* _rt, const char* signature, int* instructions) {
	convertRT(_rt);

	rt->putFunction(signature, instructions);
}

void callFunction(void* _rt, const char* signature) {
	convertRT(_rt);

	rt->callFunction(signature);
}

void runRuntime(void* _rt, const char* mainClass, const char* mainFuncSignature) {
	convertRT(_rt);

	xclass* c = rt->getClassManager()->getClass(mainClass);
	rt->run(c, mainFuncSignature);
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
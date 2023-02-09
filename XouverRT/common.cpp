#include "common.h"


#define XRT
#include <XNI.h>
#include <exception>
#include <iostream>
#include <string>
#include <fstream>

#include <runtime/runtime.h>
#include <opcodes.h>

#include <memory/memory.h>
#include <mapping/functionmap.h>

#include <class/native/nativeclasses.h>
#include <class/xclass.h>
#include <class/classmanager.h>

#include <memory/memory.h>

#define VERSION_MAJ 1
#define VERSION_MIN 0
#define XNI_VERIFY_NUM 0x2008

XNI_Error currentError = XNI_Error::NO_XNI_ERROR;
runtime* rt;

void* createRuntime(XNI_Error* error, unsigned char* bytes, int bytesCount) {
	if (bytes == nullptr) {
		*error = XNI_Error::FAILED_RT_CREATION;
		return nullptr;
	}
	
	runtime* _rt = new runtime();
	unsigned char* buffer = (unsigned char*)_rt->getMemoryManager()->allocate(sizeof(unsigned char*) * bytesCount);

	if (buffer == nullptr) {
		*error = XNI_Error::FAILED_RT_CREATION;
		return nullptr;
	}

	memcpy(buffer, bytes, bytesCount);
	_rt->bytes = buffer;

	registerFunction(_rt, "int:ToString()#void", *nativeInt::toString);
	registerFunction(_rt, "object:Equals()#void", *nativeObject::equals);

	*error = NO_XNI_ERROR;
	return rt;
}

runtime* _convertRT(void* rt) {
	try {
		return ((runtime*)rt);
	}
	catch (std::exception e) {
		currentError = XNI_Error::ARGUMENT_NOT_RT;
		return nullptr;
	}
}
#define convertRT(_rt) if ((rt = _convertRT(_rt)) == nullptr) XNI_Error::POINTER_TO_NO_RT;

XNI_Error createClass(void* _rt, unsigned char* buffer, int bytesCount) {
	convertRT(_rt);
	int ptr = 0;

	int classCount = BYTE_INT(buffer, &ptr);
	memorymanager* mem = rt->getMemoryManager();
	function_map* fmap = rt->getFunctionMap();
	
	while (classCount > 0) {
		xclass* c = (xclass*)mem->allocate(sizeof(xclass));
		int nameLen = BYTE_INT(buffer, &ptr);
		c->fullName = (char*)mem->allocate(sizeof(char) * (nameLen + 1));

		for (int i = 0; i < nameLen; i++)
			c->fullName[i] = buffer[ptr++];
		c->fullName[nameLen] = '\0';

		c->scopeSize = BYTE_INT(buffer, &ptr);
		c->functionsSize = BYTE_INT(buffer, &ptr);
		c->functions = (int*)mem->allocate(sizeof(int));
		for (int i = 0; i < c->functionsSize; i++) {
			functioninfo info;

			int signatureLen = BYTE_INT(buffer, &ptr);
			info.signature = (char*)mem->allocate(sizeof(char) * (signatureLen + 1));
			for (int j = 0; j < signatureLen; j++)
				info.signature[j] = buffer[ptr++];

			info.signature[signatureLen] = '\0';
			info.pointer = BYTE_INT(buffer, &ptr);

			fmap->putFunction(info);
		}
		c->poolSize = BYTE_INT(buffer, &ptr);
		c->pool = (xvalue**)mem->allocate(sizeof(xvalue*) * c->poolSize);
		for (int i = 0; i < c->poolSize; i++) {
			xvalue* val = (xvalue*)mem->allocate(sizeof(xvalue));

			unsigned char type = buffer[ptr++];

			if (type == 11) {
				val->type = valuetype::INT;
				val->value.i = BYTE_INT(buffer, &ptr);
			}
			else throw std::exception();

			c->pool[i] = val;
		}
		int endOff = BYTE_INT(buffer, &ptr);
		c->funcsOffset = ptr;
		rt->getClassManager()->putClass(c);
		ptr += endOff;

		classCount--;
	}
	return NO_XNI_ERROR;
}

const char* resolveRTError(XRT_Error error) {
	switch (error) {
	case (XRT_Error::EXCEPTION_THROWN): return "Exception thrown";
	case (XRT_Error::MEMORY_ALLOCATION_FAILED): return "Memory allocation failed";
	case (XRT_Error::MEMORY_INITIALIZATION_FAILED): return "Memory initialization failed";
	default: return "No error";
	}
}
const char* resolveXNIError(XNI_Error error) {
	switch (error) {
	case (XNI_Error::ARGUMENT_NOT_RT): return "Passed argumend is not a pointer to runtime";
	case (XNI_Error::POINTER_TO_NO_RT): return "Passed argumend is not a pointer to runtime";
	case (XNI_Error::UNEXPECTED_BYTE): return "Given bytes contain unexpected order";
	case (XNI_Error::NO_XNI_ERROR): return "No error";
	default: return "Inalid XNI_Error argument";
	}
}

bool xniVerify(char c) {
	return (c == XNI_VERIFY_NUM);
}

int getXMajorVersion() {
	return VERSION_MAJ;
}

int getXMinorVersion() {
	return VERSION_MIN;
}

void registerFunction(void* _rt, const char* signature, void (*fn)(void*)) {
	convertRT(_rt);

	rt->putNativeFunction(std::string(signature), fn);
}

void callFunction(void* _rt, int index) {
	convertRT(_rt);

	rt->callFunction(index);
}

void runRuntime(void* _rt, const char* mainClass, const char* mainFuncSignature) {
	convertRT(_rt);

	xclass* c = rt->getClassManager()->getClass(mainClass);
	rt->run(c, mainFuncSignature);
}

void haltRuntime(void* _rt) {
	convertRT(_rt);

	rt->halt();
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

/*const xvalues& getStackTop(void* _rt) {
	convertRT(_rt);

	return rt->getStackTop();
}*/
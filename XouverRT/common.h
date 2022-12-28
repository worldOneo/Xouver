#ifndef COMMON_H
#define COMMON_H

#include <XNI.h>
#include "class/xclass.h"	

void setXniError(XNI_Error e);

void registerFunction(void* _rt, const char* signature, unsigned char* instructions, int scopeSize);
void setExceptionMessage(const char* msg);
void registerClass(void* _rt, const char* signaure, xclass* c);

#endif
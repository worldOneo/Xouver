#ifndef COMMON_H
#define COMMON_H

#include <XNI.h>
#include "class/xclass.h"	

#define BYTE_INT(bytes, pos)		_byteint(bytes, pos)
#define BYTE_FLOAT(bytes, pos)      _bytefloat(bytes, pos)

const char* setXniError(XNI_Error e);
const char* setExceptionMessage(const char* msg);
int _byteint(unsigned char* bytes, int* pos);
float _bytefloat(unsigned char* bytes, int* pos);

#endif
#ifndef COMMON_H
#define COMMON_H

#include <XNI.h>
#include "class/xclass.h"	

#define BYTE_INT(bytes, pos)		(bytes[*pos] << 24) | (bytes[*pos + 1] << 16) | (bytes[*pos + 2] << 8) | (bytes[*pos + 3]); *pos += 4;

const char* setXniError(XNI_Error e);
const char* setExceptionMessage(const char* msg);
#endif
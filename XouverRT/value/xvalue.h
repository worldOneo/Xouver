#ifndef XVALUE_H
#define XVALUE_H

#include <XNI.h>
#include "valuetype.h"

struct xvalue {
	valuetype type;
	xvalues value;
	int refcount = 0;

	bool operator == (xvalue v) {
		xvalues val = v.value;

		if (v.type == this->type) {
			if (this->type == valuetype::FLOAT)
				return (val.f == value.f);
			else if (this->type == valuetype::INT)
				return (val.i == value.i);
			else if (this->type == valuetype::BOOL)
				return (val.b == value.b);
			else if (this->type == valuetype::CHAR)
				return (val.c == value.c);
			else if (this->type == valuetype::OBJECT)
				return (val.o == value.o);
			else
				return false;
		}

		return false;
	}
};

#endif
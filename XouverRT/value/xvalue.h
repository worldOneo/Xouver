#ifndef XVALUE_H
#define XVALUE_H

#include <XNI.h>
#include "valuetype.h"

struct xvalue {
	valuetype type;
	xvalues value;

	xvalue(xint v) {
		type = valuetype::INT;
		value.i = v;
	}
	xvalue(xfloat v) {
		type = valuetype::FLOAT;
		value.f = v;
	}
	xvalue(xbool v) {
		type = valuetype::BOOL;
		value.b = v;
	}
	xvalue(xchar v) {
		type = valuetype::BOOL;
		value.c = v;
	}
	xvalue(xobject v) {
		type = valuetype::OBJECT;
		value.o = v;
	}


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
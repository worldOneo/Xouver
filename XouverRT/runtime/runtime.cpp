#include "runtime.h"

#include <XNI.h>
#include <string>
#include <iostream>

#include <memory/memory.h>

#include <object/object.h>
#include <common.h>

#include <opcodes.h>

#define BYTES_END		(unsigned char)0x94

#define MAP_BEGIN		(unsigned char)0x74
#define POOL_BEGIN		(unsigned char)0x33

runtime::runtime() {
	/*this->bytes = new unsigned char[bytesCount];
	for (int i = 0; i < bytesCount; i++) {
		this->bytes[i] = bytes[i];
	}
	
	xclass* c = new xclass();
	int ptr = 0;

	int scopeCount = BYTE_INT(bytes, &ptr);
	scope = (xvalue**)malloc(sizeof(xvalue*) * scopeCount);

	int funcCount = BYTE_INT(bytes, &ptr);
	for (int i = 0; i < funcCount; i++) {
		functioninfo finfo = {};
		int signatureLen = BYTE_INT(bytes, &ptr);
		finfo.signature = (char*) allocate(sizeof(char) * (signatureLen+1));

		for (int i = 0; i < signatureLen; i++) {
			finfo.signature[i] = (char) bytes[ptr++];
		}
		finfo.signature[signatureLen] = '\0';

		finfo.pointer = BYTE_INT(bytes, &ptr);

		functionmap.putFunction(finfo);
	}

	int poolCount = BYTE_INT(bytes, &ptr);
	this->pool = (xvalue**) malloc(sizeof(xvalue*) * poolCount);
	
	for (int i = 0; i < poolCount; i++) {
		xvalue* val = (xvalue*)allocate(sizeof(xvalue));

		unsigned char type = bytes[ptr++];

		if (type == 11) {
			val->type = valuetype::INT;
			val->value.i = BYTE_INT(bytes, &ptr);
		}
		else throw std::exception();

		pool[i] = val;
	}
	funcsOffset = ptr;*/

	memManager = new memorymanager(this);
}

function_map* runtime::getFunctionMap() {
	return &functionmap;
}

runtime::~runtime() {
	while (stack.size() > 0) {
		memManager->deallocate(stack.top());
		stack.pop();
	}
	delete[] this->bytes;
	delete memManager;
}

void runtime::run(xclass* mainClass, std::string func) {
	_isHalted = false;
	setClass(mainClass);
	callFunction(0);

	while (!_isHalted) {
		unsigned char inst = advance();
		switch (inst) {
		case OP_ADD: {
			xvalue* val2 = stack.top();
			stack.pop();
			xvalue* val1 = stack.top();
			stack.pop();

			std::cout << val1->value.i << " + " << val2->value.i << "\n";

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.f = v1 + val2->value.f;
					v->type = valuetype::FLOAT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.i = v1 + val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			case valuetype::INT: {
				int v1 = val1->value.i;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.i = v1 + val2->value.f;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.i = v1 + val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			default:
				throw std::exception();
			}
			break;
		}
		case OP_SUB: {
			xvalue* val2 = stack.top();
			stack.pop();
			xvalue* val1 = stack.top();
			stack.pop();

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.f = v1 - val2->value.f;
					v->type = valuetype::FLOAT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.f = v1 - val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			case valuetype::INT: {
				int v1 = val1->value.i;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.i = v1 - val2->value.f;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.i = v1 - val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			default:
				throw std::exception();
			}
			break;
		}
		case OP_MUL: {
			xvalue* val2 = stack.top();
			stack.pop();
			xvalue* val1 = stack.top();
			stack.pop();

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.f = v1 * val2->value.f;
					v->type = valuetype::FLOAT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.i = v1 * val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			case valuetype::INT: {
				int v1 = val1->value.i;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.i = v1 * val2->value.f;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.i = v1 * val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			default:
				throw std::exception();
			}
			break;
		}
		case OP_DIV: {
			xvalue* val2 = stack.top();
			stack.pop();
			xvalue* val1 = stack.top();
			stack.pop();

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.f = v1 / val2->value.f;
					v->type = valuetype::FLOAT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.i = v1 / val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			case valuetype::INT: {
				int v1 = val1->value.i;
				xvalue* v = (xvalue*)memManager->allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.i = v1 / val2->value.f;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				case valuetype::INT:
					v->value.i = v1 / val2->value.i;
					v->type = valuetype::INT;
					stack.push(v);
					break;
				default:
					throw std::exception();
				}
				break;
			}
			default:
				throw std::exception();
			}
			break;
		}
		case OP_CLOAD: {
			int pos = ptrs.top();
			int constPos = getArg();
			xvalue* v = currentClass->pool[constPos];

			std::cout << "CLOAD " << v->value.i << " from " << constPos << " on " << pos << "\n";

			stack.push(v);
			break;
		}
		case OP_LINE: {
			lines.top() = advance();
			break;
		}
		case OP_CALL: {
			int funcPtr = getArg();

			callFunction(funcPtr);
			break;
		}
		case OP_RET: {
			ptrs.pop();
			functionCallVector.pop_back();

			if (ptrs.size() == 0) _isHalted = true;
			break;
		}
		case OP_JMP: {
			int ptr = ptrs.top();
			int pos = getArg();

			if (pos < 0) ptrs.top() = ptr + pos;
			else ptrs.top() += pos - 1;
			break;
		}
		case OP_IFEQ: {
			xvalue* rval = stack.top();
			stack.pop();
			xvalue* lval = stack.top();

			xvalue* v = (xvalue*)memManager->allocate(sizeof(v));
			v->type = valuetype::BOOL;
			if (rval->type == lval->type) {
				switch (lval->type) {
				case valuetype::INT:
					v->value.b = lval->value.i == rval->value.i;
					//std::cout << v->value.b << "\n";
					break;
				case valuetype::FLOAT:
					v->value.b = lval->value.f == rval->value.f;
					break;
				case valuetype::BOOL:
					v->value.b = lval->value.b == rval->value.b;
					break;
				case valuetype::CHAR:
					v->value.b = lval->value.c == rval->value.c;
					break;
				case valuetype::OBJECT:
					v->value.b = lval->value.c == rval->value.c;
					break;
				default:
					v->value.b = false;
					break;
				}
			}
			else
				v->value.b = false;

			stack.push(v);
			break;
		}
		case OP_IFLT: {
			xvalue* rval = stack.top();
			stack.pop();
			xvalue* lval = stack.top();

			xvalue* v = (xvalue*)memManager->allocate(sizeof(v));
			v->type = valuetype::BOOL;
			if (rval->type == lval->type) {
				switch (lval->type) {
				case valuetype::INT:
					v->value.b = lval->value.i < rval->value.i;
					break;
				case valuetype::FLOAT:
					v->value.b = lval->value.f < rval->value.f;
					break;
				default:
					v->value.b = false;
					break;
				}
			}
			else
				v->value.b = false;

			stack.push(v);
			break;
		}
		case OP_IFGT: {
			xvalue* rval = stack.top();
			xvalue* lval = stack.top();

			xvalue* v = (xvalue*)memManager->allocate(sizeof(v));
			v->type = valuetype::BOOL;
			if (rval->type == lval->type) {
				switch (lval->type) {
				case valuetype::INT:
					v->value.b = lval->value.i > rval->value.i;
					break;
				case valuetype::FLOAT:
					v->value.b = lval->value.f > rval->value.f;
					break;
				default:
					v->value.b = false;
					break;
				}
			}
			else
				v->value.b = false;

			stack.push(v);
			break;
		}
		case OP_IFLQ: {
			xvalue* rval = stack.top();
			xvalue* lval = stack.top();

			xvalue* v = (xvalue*)memManager->allocate(sizeof(v));
			v->type = valuetype::BOOL;
			if (rval->type == lval->type) {
				switch (lval->type) {
				case valuetype::INT:
					v->value.b = lval->value.i <= rval->value.i;
					break;
				case valuetype::FLOAT:
					v->value.b = lval->value.f <= rval->value.f;
					break;
				default:
					v->value.b = false;
					break;
				}
			}
			else
				v->value.b = false;

			stack.push(v);
			break;
		}
		case OP_IFGQ: {
			xvalue* rval = stack.top();
			xvalue* lval = stack.top();

			xvalue* v = (xvalue*)memManager->allocate(sizeof(v));
			v->type = valuetype::BOOL;
			if (rval->type == lval->type) {
				switch (lval->type) {
				case valuetype::INT:
					v->value.b = lval->value.i >= rval->value.i;
					break;
				case valuetype::FLOAT:
					v->value.b = lval->value.f >= rval->value.f;
					break;
				default:
					v->value.b = false;
					break;
				}
			}
			else
				v->value.b = false;

			stack.push(v);
			break;
		}
		case OP_NOT: {
			xvalue* b = stack.top();

			if (b->type != valuetype::BOOL) break;

			xvalue* r = (xvalue*)memManager->allocate(sizeof(xvalue));
			r->type = valuetype::BOOL;
			r->value.b = !b->value.b;

			stack.push(r);
			break;
		}
		case OP_JIF: {
			int pos = getArg();
			xvalue* b = stack.top();

			if (b->type != valuetype::BOOL) break;
			if (b->value.b == true) ptrs.top() += pos;
			break;
		}
		case OP_NEWO: {
			/*xclass* c = ((runtime*)rt)->getClass(advance());
			object* o = (object*) allocate(sizeof(object));

			int scopeSize = c->getScopeSize();
			o->classObj = c;
			o->scopeSize = scopeSize;
			o->obj = (xvalue**)allocate(sizeof(xvalue*) * scopeSize);

			xvalue* v = (xvalue*) allocate(sizeof(xvalue));
			v->type = valuetype::OBJECT;
			v->value.o = o;

			stack.push(v);*/
			break;
		}
		case OP_OLOAD: {
			int i = advance();
			xvalue* o = stack.top();

			if (o->type != valuetype::OBJECT) break;

			stack.push(((object*)o->value.o)->obj[i]);
			break;
		}
		case OP_OSTORE: {
			int i = advance();
			xvalue* v = stack.top();

			xvalue* o = stack.top();

			if (o->type != valuetype::OBJECT) break;

			v->refcount++;
			((object*)o->value.b)->obj[i]->refcount--;
			((object*)o->value.b)->obj[i] = v;
			break;
		}
		case OP_INVO: {
			/*xvalue* o = stack.top();

			if (o->type != valuetype::OBJECT) break;

			xclass* c = ((object*)o->value.o)->classObj;
			setClass(c);*/

			int idx = getArg();
			xclass* c = classManager.getClass(idx);
			setClass(c);
			break;
		}
		case OP_POP: {
			int amount = advance();

			for (int i = 0; i < amount; i++) {
				stack.pop();
			}

			break;
		}
		case OP_LOAD: {
			int pos = getArg();
			xvalue* val = localScopes.top()[pos];

			std::cout << "LOAD " << val->value.i << " in " << pos << " on " << ptrs.top() << "\n";

			stack.push(val);
			break;
		}
		case OP_STORE: {
			int pos = getArg();
			xvalue* val = stack.top();

			std::cout << "STORE " << val->value.i << " at " << pos << " on " << ptrs.top() << "\n";

			val->refcount++;
			/*if (((runtime*)rt)->scope[pos] != nullptr)
				((runtime*)rt)->scope[pos]->refcount--;*/
			localScopes.top()[pos] = val;
			break;
		}
		case OP_CLS: {
			while (stack.size() > 0)
				stack.pop();
			break;
		}
		case 0:
			_isHalted = true;
			break;
#ifdef _DEBUG
		case OP_OUT: {
			std::cout << "out ";

			xvalue* v = stack.top();

			if (v->type == valuetype::INT)
				std::cout << v->value.i;
			else if (v->type == valuetype::FLOAT)
				std::cout << v->value.f;
			else if (v->type == valuetype::CHAR)
				std::cout << v->value.c;
			else if (v->type == valuetype::BOOL)
				if (v->value.b == true)
					std::cout << "true";
				else if (v->value.b == false)
					std::cout << "false";
				else
					std::cout << "hi";
			std::cout << "\n";
			break;
		}
#endif
		default:
			unsigned char inst = bytes[ptrs.top()];
			std::cout << "Unknown opcode: " << inst << "\n";
			_isHalted = true;
			break;
		}
	}

	_isHalted = true;
}

XRT_Error runtime::getError() {
	return currentError;
}

void runtime::setError(XRT_Error error) {
	this->currentError = error;
}

std::string runtime::getException() {
	return this->exception;
}

void runtime::halt() {
	_isHalted = true;
}

void runtime::putNativeFunction(std::string signature, void (*fn)(void*)) {
	nativeFunctions.insert(std::pair<std::string, void (__cdecl*)(void*)>(signature, fn));
}

void runtime::setClass(xclass* c) {
	classes.push(c);
	currentClass = c;
}

void runtime::callFunction(int id) {
	functioninfo info = functionmap.getFunction(id);
	std::string signature = info.signature;

	if (nativeFunctions.contains(signature)) {
		this->functionCallVector.push_back(signature.c_str());
		nativeFunctions[signature](this);
		this->functionCallVector.pop_back();
	}
	else {
		this->functionCallVector.push_back(signature.c_str());

		int ptr = currentClass->funcsOffset + info.pointer;
		xvalue** scope;
		int scopeSize = BYTE_INT(bytes, &ptr);
		scope = (xvalue**)memManager->allocate(sizeof(xvalue*) * scopeSize);

		int argsCount = BYTE_INT(bytes, &ptr);
		for (int i = argsCount - 1; i >= 0; i--) {
			scope[i] = stack.top();
			stack.pop();
		}
		ptrs.push(ptr - 1);

		localScopes.push(scope);
	}
}

xvalue* runtime::getStackTop() {
	return stack.top();
}

std::string runtime::createCallStack() {
	std::string str = "";

	for (auto it = functionCallVector.rbegin(); it != functionCallVector.rend(); it++) {
		str += "  In.. " + *it + "\n";
	}


	return str;
}

std::string runtime::createExceptionMessage(std::string msg) {
	bool isEmpty = msg.empty();

	if (!isEmpty) {
		isEmpty = true;
		for (auto it = msg.begin(); it != msg.end(); it++) {
			if (!std::isspace(*it)) {
				isEmpty = false;
				break;
			}
		}
	}

	if (isEmpty) msg = "NativeException:";

	return "Xouver Exception: " + msg + '\n' + createCallStack();
}

void runtime::throwError(std::string msg) {
	this->exception = createExceptionMessage(msg);
	
	setError(XRT_Error::EXCEPTION_THROWN);
	haltRuntime(this);
}

classmanager* runtime::getClassManager() {
	return &this->classManager;
}

memorymanager* runtime::getMemoryManager() {
	return memManager;
}

unsigned char runtime::advance() {
	int ptr = ptrs.top() + 1;
	ptrs.pop();
	ptrs.push(ptr);

	unsigned char byte = bytes[ptr];
	return bytes[ptr];
}

int runtime::getArg() {
	advance();
	int arg = BYTE_INT(bytes, &ptrs.top());

	ptrs.top()--;
	return arg;
}
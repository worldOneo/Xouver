#include <XNI.h>

#include <string>
#include <iostream>

#include <opcodes.h>
#include <runtime/runtime.h>
#include <runtime/runtimeprocess.h>

#include <value/valuetype.h>
#include <value/xvalue.h>

#include <object/object.h>

#include <memory/memory.h>

runtime_process::runtime_process(void* rt) {
	this->rt = rt;
	_isHalted = false;
}

xvalue* runtime_process::run() {
	while (!_isHalted) {
		switch (advance()) {
		case OP_ADD: {
			xvalue* val2 = stack.top();
			xvalue* val1 = stack.top();

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

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
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

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
			xvalue* val1 = stack.top();

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

				switch (val2->type) {
				case valuetype::FLOAT:
					v->value.f = v1 - val2->value.f;
					v->type = valuetype::FLOAT;
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
			case valuetype::INT: {
				int v1 = val1->value.i;
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

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
			xvalue* val1 = stack.top();

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

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
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

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
			xvalue* val1 = stack.top();

			switch (val1->type) {
			case valuetype::FLOAT: {
				float v1 = val1->value.f;
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

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
				xvalue* v = (xvalue*)allocate(sizeof(xvalue));

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
			int constPos = advance();
			xclass* c = getCurrentClass();
			xvalue* v = c->getConstant(constPos);

			stack.push(v);
			break;
		}
		case OP_LINE: {
			lines.top() = advance();
			break;
		}
		case OP_CALL: {
			((runtime*)rt)->callFunction(advance());
			break;
		}
		case OP_RET: {
			ptrs.pop();
			functionCallVector.pop_back();
			instructions.pop();
		}
		case OP_JMP: {
			int pos = advance();
			ptrs.top() = pos - 1;
			break;
		}
		case OP_IFEQ: {
			xvalue* rval = stack.top();
			xvalue* lval = stack.top();

			xvalue* v = (xvalue*) allocate(sizeof(v));
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
			xvalue* lval = stack.top();

			xvalue* v = (xvalue*)allocate(sizeof(v));
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

			xvalue* v = (xvalue*)allocate(sizeof(v));
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

			xvalue* v = (xvalue*)allocate(sizeof(v));
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

			xvalue* v = (xvalue*)allocate(sizeof(v));
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

			xvalue* r = (xvalue*) allocate(sizeof(xvalue));
			r->type = valuetype::BOOL;
			r->value.b = !b->value.b;

			stack.push(r);
		}
		case OP_JIF: {
			int pos = advance();
			xvalue* b = stack.top();

			if (b->type != valuetype::BOOL) break;
			if (b->value.b == true) ptrs.top() = pos - 1;
			break;
		}
		case OP_NEWO: {
			xclass* c = ((runtime*)rt)->getClass(advance());
			object* o = (object*) allocate(sizeof(object));

			int scopeSize = c->getScopeSize();
			o->classObj = c;
			o->scopeSize = scopeSize;
			o->obj = (xvalue**)allocate(sizeof(xvalue*) * scopeSize);

			xvalue* v = (xvalue*) allocate(sizeof(xvalue));
			v->type = valuetype::OBJECT;
			v->value.o = o;

			stack.push(v);
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
			xvalue* o = stack.top();
			
			if (o->type != valuetype::OBJECT) break;

			xclass* c = ((object*)o->value.o)->classObj;
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
		case OP_LLOAD: {
			int pos = advance();

			stack.push(localScopes.top()[pos]);
			break;
		}
		case OP_LSTORE: {
			int pos = advance();
			xvalue* val = stack.top();

			val->refcount++;
			localScopes.top()[pos]->refcount--;
			localScopes.top()[pos] = val;
			break;
		}
		case OP_CLS: {
			while (stack.size() > 0)
				stack.pop();
			break;
		}
		case 8:
			delete[] instructions.top();
			_isHalted = true;
			break;
#ifdef _DEBUG
		case OP_OUT: {
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
			int inst = instructions.top()[ptrs.top()];
			std::cout << "Unknown opcode: " << instructions.top()[ptrs.top()] << "\n";
			_isHalted = true;
			break;
		}
	}

	_isHalted = true;
	return nullptr;
}


void runtime_process::setNativeFunction(std::string signature, void (*fn)(void*)) {
	this->functionCallVector.push_back(signature.c_str());
	fn(this->rt);
	this->functionCallVector.pop_back();

	if (instructions.size() < 1) {
		haltCurrentProcess(this->rt);
	}
}

void runtime_process::setFunction(std::string signature, unsigned char* instructions) {
	this->functionCallVector.push_back(signature.c_str());
	this->instructions.push(instructions);
	this->ptrs.push(-1);
}

void runtime_process::returnFunction() {
	this->instructions.pop();
	this->ptrs.pop();

	if (this->ptrs.empty()) {
		halt();
	}
}

bool runtime_process::isHalted() {
	return _isHalted;
}

void runtime_process::halt() {
	_isHalted = true;
}

void runtime_process::setClass(xclass* c) {
	classes.push(c);
}

xclass* runtime_process::getCurrentClass() {
	return classes.top();
}

std::string runtime_process::createCallStack() {
	std::string str = "";

	for (auto it = functionCallVector.rbegin(); it != functionCallVector.rend(); it++) {
		str += "  In.. " + *it + "\n";
	}
	

	return str;
}

std::string runtime_process::createExceptionMessage(std::string msg) {
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

const xvalue& runtime_process::getStackTop() {
	xvalue* v = stack.top();

	return *v;
}

unsigned char runtime_process::advance() {
	int ptr = ptrs.top() + 1;
	ptrs.pop();
	ptrs.push(ptr);
	return instructions.top()[ptr];
}
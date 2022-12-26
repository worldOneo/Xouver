#include <XNI.h>

#include <string>
#include <iostream>

#include <instructions.h>
#include <runtime/runtimeprocess.h>

#include <value/valuetype.h>
#include <value/xvalue.h>

runtime_process::runtime_process(void* rt) {
	this->rt = rt;
	_isHalted = false;
}

ptr* runtime_process::run() {
	while (!_isHalted) {
		switch (advance()) {
		case INST_ADD: {
			ptr* val2 = stack.top();
			stack.pop();
			ptr* val1 = stack.top();
			stack.pop();

			switch ((**val1).type) {
			case valuetype::FLOAT: {
				float v1 = (**val1).value.f;

				switch ((**val2).type) {
				case valuetype::FLOAT:
					stack.push(new ptr(v1 + (**val2).value.f));
					break;
				case valuetype::INT:
					stack.push(new ptr(v1 + (**val2).value.i));
					break;
				default:
					throw std::exception();
				}
			}
			case valuetype::INT: {
				int v1 = (**val1).value.i;

				switch ((**val2).type) {
				case valuetype::FLOAT:
					stack.push(new ptr(v1 + (**val2).value.f));
					break;
				case valuetype::INT:
					stack.push(new ptr(v1 + (**val2).value.i));
				default:
					throw std::exception();
				}
			}
			default:
				throw std::exception();
			}
		}
		case INST_CLOAD: {
			ptr* v = getCurrentClass()->getConstant(advance());
			stack.push(v);
			break;
		}
		case INST_LINE:
			lines.top() = advance();
			break;
		case INST_NCALL:
		//	callFunction(rt, "object:Equals()#void", {}, 0);
			break;
		case 8:
			delete[] instructions.top();
			_isHalted = true;
			break;
#ifdef _DEBUG
		case INST_OUT: {
			xvalue v = stack.top();

			if (v.type == valuetype::INT)
				std::cout << v.value.i;
			else if (v.type == valuetype::FLOAT)
				std::cout << v.value.f;
			else if (v.type == valuetype::CHAR)
				std::cout << v.value.c;
			else
				break;
			std::cout << "\n";
			break;
		}
#endif
		default:
			std::cout << "Unknown Instruction: " << instructions.top()[ptrs.top()] << "\n";
			_isHalted = true;
			break;
		}
	}

	_isHalted = true;
	return nullptr;
}


ptr* runtime_process::setNativeFunction(std::string signature, void (*fn)(void*, xfunc_data*), xvalues* args) {
	/*xfunc_data d;
	d.params = args;

	this->functionCallVector.push_back(signature.c_str());
	fn(this->rt, &d);
	this->functionCallVector.pop_back();
	ptr* p = new ptr(d.returnValue);

	if (instructions.size() < 1) {
		haltCurrentProcess(this->rt);
	}
	return p;*/

	return nullptr;
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

void runtime_process::pushFromPool(int i) {
	stack.push(getCurrentClass()->getConstant(i));
}

void runtime_process::pushFromScope(int i) {
	stack.push(localScopes.top()[i]);
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
	ptr* p = stack.top();

	return **p;
}

unsigned char runtime_process::advance() {
	int ptr = ptrs.top() + 1;
	ptrs.pop();
	ptrs.push(ptr);
	return instructions.top()[ptr];
}
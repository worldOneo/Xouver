#include "runtime.h"



#include <XNI.h>
#include <string>
#include <iostream>

#include <runtime/runtimeprocess.h>
#include <object/object.h>
#include <common.h>

runtime::~runtime() {
	for (auto process : processes) {
		delete process;
	}
}

void runtime::run(xclass* mainClass, std::string func) {
	runtime_process* main = new runtime_process(this);

	processes.push_back(main);
	current = processes.begin();

	setClass(mainClass);
	callFunction(0);

	while (current != processes.end()) {
		if (*current != nullptr) {
			if ((*current)->isHalted()) {
				delete *current;
				processes.erase(current);
				
				if (processes.empty())
					break;
			}
			else {
				(*current)->run();
			}
		}

		switchProcess();
	}
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

void runtime::putNativeFunction(std::string signature, void (*fn)(void*)) {
	nativeFunctions.insert(std::pair<std::string, void (__cdecl*)(void*)>(signature, fn));
}

void runtime::putFunction(std::string signature, int* instructions) {
	functions.insert(std::pair<std::string, int*>(signature,instructions));
}

void runtime::mapFunction(int id, std::string signature) {
	functionmap.putFunction(id, signature);
}

void runtime::setClass(xclass* c) {
	(*current)->setClass(c);
}

void runtime::mapClass(int id, std::string path) {
	classmap.putClass(id, path);
}

xclass* runtime::getClass(int id) {
	std::string path = classmap.getClass(id);
	return getClassManager()->getClass(path);
}

void runtime::callFunction(int id) {
	callFunction(functionmap.getFunction(id));
}

void runtime::callFunction(std::string signature) {
	if (nativeFunctions.contains(signature)) {
		(*current)->setNativeFunction(signature, nativeFunctions[signature]);
	}
	else {
		std::vector<xvalue*> scope;
		/*for (int i = 0; i < argCount; i++) {
			scope.push_back(new ptr(args[i]));
		}*/
		(*current)->localScopes.push(scope);
		(*current)->setFunction(signature, functions[signature]);
	}
}

bool runtime::haltProcess() {
	if (current != processes.end()) {
		(*current)->halt();
		return true;
	}

	return false;
}

const xvalue& runtime::getStackTop() {
	return (*current)->getStackTop();
}

void runtime::switchProcess() {
	current++;
}

void runtime::throwError(std::string msg) {
	this->exception = (* current)->createExceptionMessage(msg);

	setError(XRT_Error::APP_EXCPETION_THROWN);
	haltRuntime(this);
}

classmanager* runtime::getClassManager() {
	return &this->classManager;
}
#ifndef RUNTIME_H
#define RUNTIME_H

#include <XNI.h>
#include <class/xclass.h>
#include <class/classManager.h>

#include <mapping/functionmap.h>
#include <mapping/classmap.h>

#include <memory/memory.h>

#include <stack>
#include <string>
#include <map>

class runtime {
private:
	XRT_Error currentError;
	
	memorymanager* memManager;
	classmanager classManager;

	bool _isHalted;
	xclass* currentClass;

	std::stack<int> ptrs;
	std::vector<std::string> functionCallVector;
	std::stack<xclass*> classes;
	std::stack<xvalue*> stack;
	std::stack<xvalue**> localScopes;
	std::stack<int> lines;

	std::map<std::string, void (__cdecl*) (void*)> nativeFunctions;

	function_map functionmap;
	class_map classmap;

	std::string exception;
public:
	int funcOffset;
	unsigned char* bytes;

	runtime();
	~runtime();
	void run(xclass* mainClass, std::string func);

	XRT_Error getError();
	void setError(XRT_Error);

	std::string getException();

	void putNativeFunction(std::string signature, void (*fn)(void*));
	void setClass(xclass* c);
	void callFunction(int id);
	void halt();
	std::string createCallStack();
	std::string createExceptionMessage(std::string msg);
	void throwError(std::string msg);

	classmanager* getClassManager();
	memorymanager* getMemoryManager();
	function_map* getFunctionMap();
	xvalue* getStackTop();

	unsigned char advance();
	int getArg();
};

#endif
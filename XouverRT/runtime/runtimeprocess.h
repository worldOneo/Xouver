#ifndef RUNTIME_PROCESS_H
#define RUNTIME_PROCESS_H

#include <XNI.h>

#include <stack>
#include <vector>
#include <string>
#include <map>
#include <class/xclass.h>

class runtime_process {
private:
	void* rt;
	bool _isHalted;

	std::stack<int> ptrs;
	std::stack<unsigned char*> instructions;
	std::vector<std::string> functionCallVector;
	std::stack<xclass*> classes;
	std::stack<xvalue*> stack;
	std::stack<int> lines;
public:
	std::stack<xvalue**> localScopes;

	runtime_process(void* rt);

	xvalue* run();	
	void setFunction(std::string signature, unsigned char* instructions);
	void returnFunction();
	void setNativeFunction(std::string signature, void (*fn)(void*));
	bool isHalted();
	void halt();

	void setClass(xclass* c);
	xclass* getCurrentClass();

	std::string createCallStack();
	std::string createExceptionMessage(std::string msg);

	const xvalue& getStackTop();

	unsigned char advance();
};

#endif
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
	std::stack<ptr*> stack;
	std::stack<int> lines;
public:
	std::stack<std::vector<ptr*>> localScopes;

	runtime_process(void* rt);

	ptr* run();	
	void setFunction(std::string signature, unsigned char* instructions);
	void returnFunction();
	ptr* setNativeFunction(std::string signature, void (*fn)(void*, xfunc_data*), xvalues* args);
	bool isHalted();
	void halt();

	void pushFromPool(int i);
	void pushFromScope(int i);

	void setClass(xclass* c);
	xclass* getCurrentClass();

	std::string createCallStack();
	std::string createExceptionMessage(std::string msg);

	const xvalue& getStackTop();

	unsigned char advance();
};

#endif
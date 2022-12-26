#ifndef RUNTIME_H
#define RUNTIME_H

#include <XNI.h>
#include <runtime/runtimeprocess.h>
#include <class/xclass.h>
#include <class/classManager.h>

#include <stack>
#include <string>
#include <map>

class runtime {
private:
	XRT_Error currentError;
	std::vector<runtime_process*>::iterator current;
	std::vector<runtime_process*> processes;
	classmanager classManager;

	std::map<std::string, void (__cdecl*) (void*, xfunc_data*)> nativeFunctions;
	std::map<std::string, unsigned char*> functions;

	std::string exception;
public:
	~runtime();
	void run(xclass* mainClass, std::string func);

	XRT_Error getError();
	void setError(XRT_Error);

	std::string getException();

	void putNativeFunction(std::string signature, void (*fn)(void*, xfunc_data*));
	void putFunction(std::string signature, unsigned char* instructions);
	void setClass(xclass* c);
	void callFunction(std::string signature, xvalue* args, int argCount);
	bool haltProcess();
	void switchProcess();
	void throwError(std::string msg);

	classmanager* getClassManager();
	const xvalue& getStackTop();
};

#endif
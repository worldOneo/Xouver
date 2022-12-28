#ifndef RUNTIME_H
#define RUNTIME_H

#include <XNI.h>
#include <runtime/runtimeprocess.h>
#include <class/xclass.h>
#include <class/classManager.h>

#include <mapping/functionmap.h>
#include <mapping/classmap.h>

#include <stack>
#include <string>
#include <map>

class runtime {
private:
	XRT_Error currentError;
	std::vector<runtime_process*>::iterator current;
	std::vector<runtime_process*> processes;
	classmanager classManager;

	std::map<std::string, void (__cdecl*) (void*)> nativeFunctions;
	std::map<std::string, int*> functions;
	function_map functionmap;
	class_map classmap;

	std::string exception;
public:
	~runtime();
	void run(xclass* mainClass, std::string func);

	XRT_Error getError();
	void setError(XRT_Error);

	std::string getException();

	void putNativeFunction(std::string signature, void (*fn)(void*));
	void putFunction(std::string signature, int* instructions);
	void mapFunction(int id, std::string signature);
	void setClass(xclass* c);
	void mapClass(int id, std::string path);
	xclass* getClass(int id);
	void callFunction(int id);
	void callFunction(std::string signature);
	bool haltProcess();
	void switchProcess();
	void throwError(std::string msg);

	classmanager* getClassManager();
	const xvalue& getStackTop();
};

#endif
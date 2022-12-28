#ifndef CLASS_MANAGER_H
#define CLASS_MANAGER_H

#include <map>
#include <string>

#include "xclass.h"

class classmanager {
private:
	std::map<std::string, xclass*> classes;

public:
	void putClass(std::string path, xclass* c);
	xclass* getClass(std::string path);
	void clearClasses();
};

#endif
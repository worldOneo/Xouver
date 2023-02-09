#ifndef CLASS_MANAGER_H
#define CLASS_MANAGER_H

#include <vector>
#include <string>

#include "xclass.h"

class classmanager {
private:
	std::vector<xclass*> classes;

public:
	void putClass(xclass* c);
	xclass* getClass(int index);
	xclass* getClass(std::string path);
	void clearClasses();
};

#endif
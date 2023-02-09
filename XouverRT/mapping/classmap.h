#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <vector>

typedef struct s_classinfo {
	char* signature;
	int pointer;
} classinfo;

class class_map {
private:
	std::vector<classinfo> classes;
public:
	classinfo& getClass(int index);
	void putClass(classinfo info);
};

#endif
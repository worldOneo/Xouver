#include "classmanager.h"

#include <string>

xclass* classmanager::getClass(int index) {
	return classes[index];
}

xclass* classmanager::getClass(std::string path) {
	for (auto it = classes.begin(); it != classes.end(); it++) {
		if ((*it)->fullName == path) return *it;
	}

	return nullptr;
}

void classmanager::putClass(xclass* c) {
	classes.push_back(c);
}

void classmanager::clearClasses() {
	for (auto it = classes.begin(); it != classes.end(); it++) {
		free(*it);
		classes.erase(it);
	}
}
#include "classmanager.h"

#include <string>

xclass* classmanager::getClass(std::string path) {
	return classes[path];
}

void classmanager::putClass(std::string path, xclass* c) {
	classes.emplace(std::pair<std::string, xclass*>(path, c));
}

void classmanager::clearClasses() {
	for (auto it = classes.begin(); it != classes.end(); it++) {
		free(it->second);
		classes.erase(it);
	}
}
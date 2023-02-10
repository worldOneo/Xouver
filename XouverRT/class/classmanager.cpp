#include "classmanager.h"

#include <string>

xclass* classmanager::getClass(int index) {
	return &classes[index];
}

xclass* classmanager::getClass(std::string path) {
	for (auto &it : classes) {
		if (it.fullName == path)
			return &it;
	}

	return nullptr;
}

void classmanager::putClass(xclass c) {
	classes.push_back(c);
}

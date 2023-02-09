#include "classmap.h"

#include <memory/memory.h>

classinfo& class_map::getClass(int index) {
	return classes[index];
}

void class_map::putClass(classinfo info) {
	classes.push_back(info);
}
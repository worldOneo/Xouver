#include "classmap.h"

std::string class_map::getClass(int id) {
	for (auto it : map) {
		if (it.first == id) return it.second;
	}

	return "";
}

void class_map::putClass(int id, std::string signature) {
	map.emplace(std::pair(id, signature));
}
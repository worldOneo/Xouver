#include "functionmap.h"

std::string function_map::getFunction(int id) {
	for (auto it : map) {
		if (it.first == id) return it.second;
	}

	return "";
}

void function_map::putFunction(int id, std::string signature) {
	map.emplace(std::make_pair(id, signature));
}
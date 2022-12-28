#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <map>
#include <string>

class class_map {
private:
	std::map<int, std::string> map;
public:
	std::string getClass(int id);
	void putClass(int id, std::string path);
};

#endif
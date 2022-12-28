#ifndef FUNCTIONMAP_H
#define FUNCTIONMAP_H

#include <map>
#include <string>

class function_map {
private:
	std::map<int, std::string> map;
public:
	std::string getFunction(int id);
	void putFunction(int id, std::string signature);
};

#endif
#ifndef FUNCTIONMAP_H
#define FUNCTIONMAP_H

#include <string>
#include <vector>


typedef struct s_functioninfo {
	int pointer;
	std::string signature;
} functioninfo;

class function_map {
private:
	std::vector<functioninfo> functions;
public:
	functioninfo& getFunction(int index);
	void putFunction(functioninfo info);
};

#endif
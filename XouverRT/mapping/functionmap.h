#ifndef FUNCTIONMAP_H
#define FUNCTIONMAP_H

#include <vector>

typedef struct s_functioninfo {
	char* signature;
	int pointer;
} functioninfo;

class function_map {
private:
	std::vector<functioninfo> functions;
public:
	functioninfo& getFunction(int index);
	void putFunction(functioninfo info);
};

#endif
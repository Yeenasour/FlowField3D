#ifndef FLOWFIELD_EXPRESSION
#define FLOWFIELD_EXPRESSION

#include <string>

class Expression
{
private:
	std::string expression;
	bool ok;
public:
	Expression(std::string e);
	~Expression();
	float eval(float x, float y, float z);
};

#endif
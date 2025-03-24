#ifndef FLOWFIELD_EXPRESSION
#define FLOWFIELD_EXPRESSION

#include <string>

class Expression
{
private:
	std::string expression;
public:
	Expression(const std::string &e);
	~Expression();
	float eval(float x, float y, float z);
};

#endif
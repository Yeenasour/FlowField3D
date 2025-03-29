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
	void setExpression(const std::string &e);
	float eval(float x, float y, float z);
};

#endif
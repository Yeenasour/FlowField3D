#ifndef FLOWFIELD_EXPRESSION
#define FLOWFIELD_EXPRESSION

#include <string>

class Expression
{
private:
	/* constants in the expression are denoted by a character whose MSB == 1
	*/
	std::string expression;
public:
	Expression(const std::string &e);
	~Expression();
	void setExpression(const std::string &e);
	float eval(float x, float y, float z);
};

#endif
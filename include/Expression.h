#ifndef FLOWFIELD_EXPRESSION
#define FLOWFIELD_EXPRESSION

#include <string>


class Expression
{
private:
	float constant;
	std::string expression;
public:
	Expression(const std::string &e);
	~Expression();
	void setExpression(const std::string &e);
	void setConstant(float val);
	float eval(float x, float y, float z) const;
};

#endif
#ifndef FLOWFIELD_EXPRESSION_PARSER
#define FLOWFIELD_EXPRESSION_PARSER

#include <Expression.h>
#include <vector>
#include <stack>
#include <stdexcept>


class ExpressionParser
{
private:
	static bool isVar(char c);
	static bool isOperator(char c);
	static int precedence(char op);
	static bool isLeftAssociative(char op);
	static std::vector<std::string> split(std::string &fieldExpression);
	static std::string toPolish(std::string &expression);
public:
	static std::vector<Expression> getVectorFieldExpressions(std::string &fieldExpression);
};

#endif
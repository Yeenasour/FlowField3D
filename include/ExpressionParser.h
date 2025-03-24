#ifndef FLOWFIELD_EXPRESSION_PARSER
#define FLOWFIELD_EXPRESSION_PARSER

#include <string>
#include <vector>
#include <stack>


class ExpressionParser
{
private:
	static bool isVar(char c);
	static bool isOperator(char c);
	static int precedence(char op);
	static bool isLeftAssociative(char op);
public:
	static std::vector<std::string> split(std::string &fieldExpression);
	static std::string toPolish(std::string &expression);
};

#endif
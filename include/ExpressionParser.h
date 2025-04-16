#ifndef FLOWFIELD_EXPRESSION_PARSER
#define FLOWFIELD_EXPRESSION_PARSER

#include <string>
#include <vector>


class Expression;

class ExpressionParser
{
private:
	static bool isFunc(char c);
	static bool isVar(char c);
	static bool isOperator(char c);
	static int precedence(char op);
	static bool isLeftAssociative(char op);
	static std::vector<std::string> split(const std::string &fieldExpression);
	static std::string toPolish(const std::string &expression);
	static char encodeNumChar(const std::string &expression, int start, int end);
	static bool isNumEncoded(char c);
public:
	static std::vector<Expression> getVectorFieldExpressions(const std::string &fieldExpression);
};

#endif
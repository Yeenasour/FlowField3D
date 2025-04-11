#include <ExpressionParser.h>
#include <Expression.h>
#include <charconv>
#include <stack>
#include <stdexcept>


std::vector<std::string> ExpressionParser::split(const std::string &fieldExpression)
{
	std::vector<std::string> expressions;
	expressions.reserve(3);
	unsigned int begin = 0, end = 0;
	unsigned int len = fieldExpression.length();
	while (begin < len && end < len)
	{
		while (end < len && fieldExpression.at(end) != ',') end++;
		std::string expression;
		expression.reserve(len / 3);
		std::string numBuff;
		numBuff.reserve(3);
		unsigned int i = begin;
		while (i < end)
		{
			char c = fieldExpression.at(i);
			if (std::isdigit(c)) {i++; continue;}
			if (i != begin)
			{
				expression += encodeNumChar(fieldExpression, begin, begin + i);
			}
			if (c != ' ') expression += c;
			begin = ++i;
		}
		if (begin != end) expression += encodeNumChar(fieldExpression, begin, end);
		if (!expression.empty()) expressions.push_back(expression);
		begin = ++end;
	}
	return expressions;
}

char ExpressionParser::encodeNumChar(const std::string &expression, int start, int end)
{
	char val;
	std::from_chars(expression.data() + start, expression.data() + end, val, 10);
	val = (val & 0x7F) | 0x80;
	return val;
}

std::string ExpressionParser::toPolish(const std::string &expression)
{
	std::string out = "";
	std::stack<char> os = std::stack<char>();
	for (size_t i = 0; i < expression.length(); i++)
	{
		char c = expression.at(i);
		if (isVar(c))
		{
			out += c;
		}
		else if (c & 0x80)
		{
			out += c;
		}
		else if (isOperator(c))
		{
			while (!os.empty() && os.top() != '(' && (precedence(os.top()) > precedence(c) || (precedence(os.top()) == precedence(c) && isLeftAssociative(c))))
			{
				out += os.top();
				os.pop();
			}
			os.push(c);
		}
		else if (c == '(')
		{
			os.push(c);
		}
		else if (c == ')')
		{
			while (!os.empty() && os.top() != '(')
			{
				out += os.top();
				os.pop();
			}
			if (os.empty())
			{
				throw std::invalid_argument("Invalid expression");
			}
			if (os.top() == '(') os.pop();
			if (!os.empty() && isFunc(os.top()))
			{
				out += os.top();
				os.pop();
			}
		}
		else if (isFunc(c))
		{
			os.push(c);
		}
		else
		{
			throw std::invalid_argument("Invalid expression");
		}
	}
	while (!os.empty())
	{
		if (os.top() == '(')
			throw std::invalid_argument("Invalid expression");
		out += os.top();
		os.pop();
	}
	return out;
}

bool ExpressionParser::isFunc(char c)
{
	return (c == 's' || c == 'c');
}

bool ExpressionParser::isVar(char c)
{
	return (c == 'x' || c == 'y' || c == 'z' || c == 'r' || c == 'R');
}

bool ExpressionParser::isOperator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int ExpressionParser::precedence(char op)
{
    switch (op) {
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
            return 3;
        case '^':
            return 4;
        default:
            return -1;
    }
}

bool ExpressionParser::isLeftAssociative(char op)
{
    return (op == '+' || op == '-' || op == '*' || op == '/');
}

std::vector<Expression> ExpressionParser::getVectorFieldExpressions(const std::string &fieldExpression)
{
	std::vector<Expression> out = std::vector<Expression>();
	std::vector<std::string> expressions = split(fieldExpression);
	for (auto &&expression : expressions)
	{
		std::string polishExpression = toPolish(expression);
		out.emplace_back(polishExpression);
	}
	return out;
}
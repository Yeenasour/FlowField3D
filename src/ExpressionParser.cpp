#include <ExpressionParser.h>

std::vector<std::string> ExpressionParser::split(std::string &fieldExpression)
{
	std::vector<std::string> expressions = std::vector<std::string>();
	unsigned int begin = 0;
	unsigned int end = 0;
	int len = fieldExpression.length();
	while (begin < len && end < len)
	{
		while (end < len && fieldExpression.at(end) != ',') end++;
		std::string expression = "";
		for (size_t i = begin; i < end; i++)
		{
			char c = fieldExpression.at(i);
			if (c != ' ') expression += c;
		}
		if (!expression.empty())
			expressions.push_back(expression);
		end++;
		begin = end;
	}
	return expressions;
}

std::string ExpressionParser::toPolish(std::string &expression)
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

bool ExpressionParser::isVar(char c)
{
	return (c == 'x' || c == 'y' || c == 'z');
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

std::vector<Expression> ExpressionParser::getVectorFieldExpressions(std::string &fieldExpression)
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
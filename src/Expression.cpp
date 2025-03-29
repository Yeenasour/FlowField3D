#include <Expression.h>
#include <stdexcept>

Expression::Expression(const std::string &e) : expression(e)
{

}

Expression::~Expression()
{

}

void Expression::setExpression(const std::string &e)
{
	this->expression = e;
}

float Expression::eval(float x, float y, float z)
{
	constexpr int MAX_STACK_SIZE = 16;
	if (this->expression == "") return 0;
	float res[MAX_STACK_SIZE];
	int top = 0;
	for (auto &&c : this->expression)
	{
		if (c == 'x' || c == 'y' || c == 'z')
		{
			float val;
			switch (c)
			{
			case 'x':
				val = x;
				break;
			case 'y':
				val = y;
				break;
			case 'z':
				val = z;
				break;
			}
			if (top == MAX_STACK_SIZE) throw std::runtime_error("Stack overflow");
			res[top++] = val;
		}
		else
		{
			if (top < 2) throw std::runtime_error("Stack underflow");
			float b = res[--top];
			float a = res[--top];
			switch (c)
			{
			case '+': res[top++] = a + b; break;
			case '-': res[top++] = a - b; break;
			case '*': res[top++] = a * b; break;
			case '/':
				if (b < 1e-6 && b > -1e-6) throw std::runtime_error("Division by zero");
				res[top++] = a / b; break;
			}
		}
	}
	return res[0];
}
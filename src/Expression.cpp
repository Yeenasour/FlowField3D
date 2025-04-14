#include <Expression.h>
#include <stdexcept>
#include <glm/glm.hpp>


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

float Expression::eval(float x, float y, float z) const
{
	constexpr int MAX_STACK_SIZE = 16;
	if (this->expression == "") return 0;
	float res[MAX_STACK_SIZE];
	int top = 0;
	for (auto &&c : this->expression)
	{
		if (c == 'x' || c == 'y' || c == 'z' || c == 'r' || c == 'R')
		{
			float val;
			switch (c)
			{
			case 'x': val = x; break;
			case 'y': val = y; break;
			case 'z': val = z; break;
			case 'r':
				val = glm::sqrt(x*x + y*y + z*z);
				break;
			case 'R':
				val = x*x + y*y + z*z;
				break;
			}
			if (top == MAX_STACK_SIZE) throw std::runtime_error("Stack overflow");
			res[top++] = val;
		}
		else if (c & 0x80)
		{
			if (top == MAX_STACK_SIZE) throw std::runtime_error("Stack overflow");
			res[top++] = (c & 0x7F);
		}
		else if (c == 's' || c == 'c')
		{
			if (top < 1) throw std::runtime_error("Stack underflow");
			float a = res[--top];
			switch (c)
			{
				case 's': res[top++] = glm::sin(a); break;
				case 'c': res[top++] = glm::cos(a); break;
			}
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
				case '^': res[top++] = glm::pow(a,b); break;
			}
		}
	}
	if (top != 1) throw std::runtime_error("Incorrect formating");
	return res[0];
}
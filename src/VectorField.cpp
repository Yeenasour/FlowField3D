#include <VectorField.h>

VectorField::VectorField(const std::string &fieldExpression)
    : xe(""), ye(""), ze("")
{
    try
    {
        std::vector<Expression> expressions = ExpressionParser::getVectorFieldExpressions(fieldExpression);
        if (expressions.size() == 3)
        {
            this->xe = expressions.at(0);
            this->ye = expressions.at(1);
            this->ze = expressions.at(2);
        }
    }
    catch(const std::exception& e) {}
}

VectorField::~VectorField()
{

}

void VectorField::setField(const std::string &fieldExpression)
{
	try
    {
        std::vector<Expression> expressions = ExpressionParser::getVectorFieldExpressions(fieldExpression);
        if (expressions.size() == 3)
        {
            this->xe = expressions.at(0);
            this->ye = expressions.at(1);
            this->ze = expressions.at(2);
        }
    }
    catch(const std::exception& e) {
		this->xe.setExpression("");
		this->ye.setExpression("");
		this->ze.setExpression("");
	}
}

glm::vec3 VectorField::evalAt(float x, float y, float z)
{
	try
	{
		float xVal = xe.eval(x, y, z);
    	float yVal = ye.eval(x, y, z);
   		float zVal = ze.eval(x, y, z);
		return glm::vec3(xVal, yVal, zVal);
	}
	catch(const std::exception& e)
	{
		return glm::vec3(0.0f);
	}
}
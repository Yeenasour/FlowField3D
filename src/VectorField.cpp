#include <VectorField.h>
#include <ExpressionParser.h>
#include <stdexcept>


VectorField::VectorField(const std::string &fieldExpression)
    : constant(1.0f), coordinateScale(1.0f), xe(""), ye(""), ze("")
{
    try
    {
        std::vector<Expression> expressions = ExpressionParser::getVectorFieldExpressions(fieldExpression);
        if (expressions.size() == 3)
        {
            this->xe = expressions.at(0);
            this->ye = expressions.at(1);
            this->ze = expressions.at(2);
			setFieldConstant();
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
        if (expressions.size() != 3) throw std::runtime_error("Not correct number of valid expressions");
        this->xe = expressions.at(0);
        this->ye = expressions.at(1);
        this->ze = expressions.at(2);
		setFieldConstant();
    }
    catch(const std::exception& e) {
		this->xe.setExpression("");
		this->ye.setExpression("");
		this->ze.setExpression("");
	}
}

void VectorField::setScale(float scale)
{
	coordinateScale = scale;
}

void VectorField::setFieldConstant()
{
	xe.setConstant(constant);
	ye.setConstant(constant);
	ze.setConstant(constant);
}

void VectorField::setFieldConstant(float val)
{
	constant = val;
	xe.setConstant(val);
	ye.setConstant(val);
	ze.setConstant(val);
}

glm::vec3 VectorField::evalAt(float x, float y, float z) const
{
	try
	{
		x *= coordinateScale;
		y *= coordinateScale;
		z *= coordinateScale;
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
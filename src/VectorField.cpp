#include <VectorField.h>

VectorField::VectorField(std::string &fieldExpression)
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

glm::vec3 VectorField::evalAt(float x, float y, float z)
{
    float xVal = xe.eval(x, y, z);
    float yVal = ye.eval(x, y, z);
    float zVal = ze.eval(x, y, z);
    return glm::vec3(xVal, yVal, zVal);
}
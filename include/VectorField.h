#ifndef FLOWFIELD_VECTORFIELD
#define FLOWFIELD_VECTORFIELD

#include <Expression.h>
#include <ExpressionParser.h>
#include <glm/glm.hpp>

class VectorField
{
private:
    Expression xe;
    Expression ye;
    Expression ze;
public:
    VectorField(std::string &fieldExpression);
    ~VectorField();
    glm::vec3 evalAt(float x, float y, float z);
};

#endif
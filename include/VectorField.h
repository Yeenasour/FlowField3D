#ifndef FLOWFIELD_VECTORFIELD
#define FLOWFIELD_VECTORFIELD

#include <Expression.h>
#include <glm/glm.hpp>


class VectorField
{
private:
    Expression xe;
    Expression ye;
    Expression ze;
public:
    VectorField(const std::string &fieldExpression);
    ~VectorField();
	void setField(const std::string &fieldExpression);
    glm::vec3 evalAt(float x, float y, float z) const;
};

#endif
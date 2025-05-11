#ifndef FLOWFIELD_VECTORFIELD
#define FLOWFIELD_VECTORFIELD

#include <Expression.h>
#include <glm/glm.hpp>


class VectorField
{
private:
	float constant;
	float coordinateScale;
    Expression xe;
    Expression ye;
    Expression ze;
	void setFieldConstant();
public:
    VectorField(const std::string &fieldExpression);
    ~VectorField();
	void setScale(float scale);
	void setField(const std::string &fieldExpression);
	void setFieldConstant(float val);
    glm::vec3 evalAt(float x, float y, float z) const;
};

#endif
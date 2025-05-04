#ifndef FLOWFIELD_AXES
#define FLOWFIELD_AXES

#include <glm/glm.hpp>
#include <Engine/Renderable.h>
#include <Engine/VertexArray.h>


typedef unsigned int GLuint;
class Camera;
class Shader;

class Axes : public Renderable
{
private:
	float length;
	void initAxes();
	void updateModelMatrix();
public:
	Axes(float size);
	~Axes() override;
	void setLength(float len);
};

#endif
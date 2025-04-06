#ifndef FLOWFIELD_AXES
#define FLOWFIELD_AXES

#include <glm/glm.hpp>


typedef unsigned int GLuint;
class Camera;
class Shader;

class Axes
{
private:
	GLuint VAO, VBO, EBO;
	float length;
	glm::mat4 modelMatrix;
	void initAxes();
	void updateModelMatrix();
public:
	Axes(float size);
	~Axes();
	void setLength(float len);
	void Draw(Camera &camera, Shader &shaderProgram);
};

#endif
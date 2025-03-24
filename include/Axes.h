#ifndef FLOWFIELD_AXES
#define FLOWFIELD_AXES

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Camera.h>
#include <Shader.h>

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
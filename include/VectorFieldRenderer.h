#ifndef FLOWFIELD_VECTORFIELD_RENDERER
#define FLOWFIELD_VECTORFIELD_RENDERER

#include <GL/glew.h>
#include <VectorField.h>
#include <glm/glm.hpp>
#include <Camera.h>
#include <Shader.h>

class VectorFieldRenderer
{
private:
    VectorField& vf;
    GLuint VAO, VBO, EBO;
	int segments;
	int vectorDensity;
	int numIndecies;
	void initBuffers();
public:
    VectorFieldRenderer(VectorField &field, int segments, int density);
    ~VectorFieldRenderer();
	void updateBuffers();
    void Draw(Camera &camera, Shader &shaderProgram);
};

#endif
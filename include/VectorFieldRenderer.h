#ifndef FLOWFIELD_VECTORFIELD_RENDERER
#define FLOWFIELD_VECTORFIELD_RENDERER

#include <glm/glm.hpp>


typedef unsigned int GLuint;
class VectorField;
class Camera;
class Shader;

class VectorFieldRenderer
{
private:
    VectorField& vf;
    GLuint VAO, VBO, EBO;
	int segments;
	int vectorDensity;
	float drawingRange;
	int numIndecies;
	void initBuffers();
public:
    VectorFieldRenderer(VectorField &field, int density, float range, int segments);
    ~VectorFieldRenderer();
	void updateBuffers();
    void Draw(Camera &camera, Shader &shaderProgram);
};

#endif
#ifndef FLOWFIELD_VECTORFIELD_RENDERER
#define FLOWFIELD_VECTORFIELD_RENDERER

#include <GL/glew.h>
#include <VectorField.h>
#include <glm/glm.hpp>


class VectorFieldRenderer
{
private:
    VectorField vf;
    GLuint VAO, VBO, EBO;
public:
    VectorFieldRenderer();
    ~VectorFieldRenderer();
    void Draw(Camera &camera, Shader &shaderProgram);
};

#endif
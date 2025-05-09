#ifndef FLOWFIELD_VECTORFIELD_RENDERER
#define FLOWFIELD_VECTORFIELD_RENDERER

#include <Engine/Renderable.h>
#include <glm/glm.hpp>


typedef unsigned int GLuint;
class VectorField;
class Camera;
class Shader;

class VectorFieldRenderer : public Renderable
{
private:
    VectorField& vf;
	int segments;
	int vectorDensity;
	float drawingRange;
	void initBuffers();
public:
    VectorFieldRenderer(VectorField &field, int density, float range);
    ~VectorFieldRenderer();
	void updateBuffers();
};

#endif
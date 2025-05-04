#ifndef ENGINE_RENDERABLE
#define ENGINE_RENDERABLE

#include <Engine/VertexArray.h>
#include <glm/glm.hpp>


class Renderable
{
protected:
	VertexArray* VAO;
	glm::mat4 modelMatrix;
public:
	Renderable()
	{
		VAO = new VertexArray();
		modelMatrix = glm::mat4(1.0f);
	}

	virtual ~Renderable()
	{
		delete VAO;
	}

	virtual glm::mat4 getModelMatrix() const
	{
		return modelMatrix;
	}

	virtual VertexArray* getVAO() const
	{
		return VAO;
	}
};

#endif
#include <Engine/Renderer.h>
#include <Engine/VertexArray.h>
#include <Engine/Buffer.h>


GLenum Renderer::toGlPrimitive(PrimitiveType type)
{
	switch (type)
	{
		case PrimitiveType::Line: 		return GL_LINES;
		case PrimitiveType::Triangle: 	return GL_TRIANGLES;
	}
	return -1;
}

void Renderer::init()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawIndexed(const VertexArray& VA, const Shader& shader, PrimitiveType type)
{
	shader.use();

	VA.bind();
	GLenum primitive = toGlPrimitive(type);
	if (primitive < 0) return;
	glDrawElements(primitive, VA.getEBO()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawInstanced(const VertexArray& VA, const Shader& shader, PrimitiveType type, int count)
{
	shader.use();

	VA.bind();
	GLenum primitive = toGlPrimitive(type);
	if (primitive < 0) return;
	glDrawElementsInstanced(primitive, VA.getEBO()->getCount(), GL_UNSIGNED_INT, nullptr, count);
}
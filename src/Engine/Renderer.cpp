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
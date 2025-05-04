#include <Engine/Renderer.h>
#include <Engine/VertexArray.h>
#include <Engine/Buffer.h>
#include <Engine/Renderable.h>


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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawIndexed(const Renderable& object, const Shader& shader, PrimitiveType type)
{
	shader.use();

	object.getVAO()->bind();
	GLenum primitive = toGlPrimitive(type);
	if (primitive < 0) return;
	glDrawElements(primitive, object.getVAO()->getEBO()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawInstanced(const Renderable& object, const Shader& shader, PrimitiveType type, int count)
{
	shader.use();

	object.getVAO()->bind();
	GLenum primitive = toGlPrimitive(type);
	if (primitive < 0) return;
	glDrawElementsInstanced(primitive, object.getVAO()->getEBO()->getCount(), GL_UNSIGNED_INT, nullptr, count);
}
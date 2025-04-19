#include <Engine/VertexArray.h>
#include <Engine/Buffer.h>
#include <GL/glew.h>


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &handle);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &handle);
}

void VertexArray::bind() const
{
	glBindVertexArray(handle);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::setAttribPointer(int index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset) const
{
	glVertexAttribPointer(index, size, type, normalized, stride, offset);
}

void VertexArray::enableAttribPointer(int index) const
{
	glEnableVertexAttribArray(index);
}

void VertexArray::disableAttribPointer(int index) const
{
	glDisableVertexAttribArray(index);
}

void VertexArray::setAttribDivisor(int index, GLuint divisor) const
{
	glVertexAttribDivisor(index, divisor);
}

void VertexArray::setVBO(VertexBuffer* vertexBuffer)
{
	glBindVertexArray(handle);
	vertexBuffer->bind();
	VBO = vertexBuffer;
}

void VertexArray::setEBO(IndexBuffer* indexBuffer)
{
	glBindVertexArray(handle);
	indexBuffer->bind();
	EBO = indexBuffer;
}

VertexBuffer* VertexArray::getVBO() const
{
	return this->VBO;
}

IndexBuffer* VertexArray::getEBO() const
{
	return this->EBO;
}
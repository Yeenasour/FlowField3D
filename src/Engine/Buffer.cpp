#include <Engine/Buffer.h>
#include <GL/glew.h>


StaticVertexBuffer::StaticVertexBuffer(void* vertices, unsigned int size)
{
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

StaticVertexBuffer::~StaticVertexBuffer()
{
	glDeleteBuffers(1, &handle);
}

void StaticVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void StaticVertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


DynamicVertexBuffer::DynamicVertexBuffer(void* vertices, unsigned int size)
{
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

DynamicVertexBuffer::~DynamicVertexBuffer()
{
	glDeleteBuffers(1, &handle);
}

void DynamicVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void DynamicVertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicVertexBuffer::subData(void* vertices, unsigned int size)
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}


IndexBuffer::IndexBuffer(unsigned int* vertices, unsigned int size)
	: count(size / sizeof(unsigned int))
{
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &handle);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
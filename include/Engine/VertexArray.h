#ifndef ENGINE_VERTEXARRAY
#define ENGINE_VERTEXARRAY

#include <vector>


typedef unsigned char GLboolean;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;
typedef int GLsizei;

class VertexBuffer;
class IndexBuffer;

class VertexArray
{
private:
	GLuint handle;
	//VertexBuffer* VBO;
	std::vector<VertexBuffer*> VBOs;
	IndexBuffer* EBO;
public:
	VertexArray();
	~VertexArray();
	void bind() const;
	void unbind() const;
	void setAttribPointer(int index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset) const;
	void enableAttribPointer(int index) const;
	void disableAttribPointer(int index) const;
	void setAttribDivisor(int index, GLuint divisor) const;
	void addVBO(VertexBuffer* vertexBuffer);
	void setEBO(IndexBuffer* indexBuffer);
	VertexBuffer* getVBO(unsigned int index) const;
	IndexBuffer* getEBO() const;
};

#endif
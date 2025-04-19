#ifndef FLOWFIELD_VERTEXARRAY
#define FLOWFIELD_VERTEXARRAY


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
	VertexBuffer* VBO;
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
	void setVBO(VertexBuffer* vertexBuffer);
	void setEBO(IndexBuffer* indexBuffer);
	VertexBuffer* getVBO() const;
	IndexBuffer* getEBO() const;
};

#endif
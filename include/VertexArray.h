#ifndef FLOWFIELD_VERTEXARRAY
#define FLOWFIELD_VERTEXARRAY


typedef unsigned char GLboolean;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;
typedef int GLsizei;

class VertexArray
{
private:
	GLuint handle;
public:
	VertexArray();
	~VertexArray();
	void bind() const;
	void unbind() const;
	void setAttribPointer(int index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset) const;
	void enableAttribPointer(int index) const;
	void disableAttribPointer(int index) const;
	void setAttribDivisor(int index, GLuint divisor) const;
};

#endif
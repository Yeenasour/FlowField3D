#ifndef FLOWFIELD_BUFFER
#define FLOWFIELD_BUFFER



typedef unsigned int GLuint;

class StaticVertexBuffer
{
private:
	GLuint handle;
public:
	StaticVertexBuffer(void* vertices, unsigned int size);
	~StaticVertexBuffer();
	void bind() const;
	void unbind() const;
};

class DynamicVertexBuffer
{
private:
	GLuint handle;
public:
	DynamicVertexBuffer(void* vertices, unsigned int size);
	~DynamicVertexBuffer();
	void bind() const;
	void unbind() const;
	void subData(void* vertices, unsigned int size);
};

class IndexBuffer
{
private:
	GLuint handle;
public:
	IndexBuffer(unsigned int* indices, unsigned int size);
	~IndexBuffer();
	void bind() const;
	void unbind() const;
};


#endif
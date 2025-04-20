#ifndef ENGINE_BUFFER
#define ENGINE_BUFFER



typedef unsigned int GLuint;

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
};

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
	unsigned int count;
public:
	IndexBuffer(unsigned int* indices, unsigned int size);
	~IndexBuffer();
	void bind() const;
	void unbind() const;
	unsigned int getCount() const { return count; }
};


#endif
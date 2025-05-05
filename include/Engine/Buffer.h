#ifndef ENGINE_BUFFER
#define ENGINE_BUFFER



typedef unsigned int GLuint;

class VertexBuffer
{
public:
	virtual ~VertexBuffer() {};
	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	virtual void subData(void* vertices, unsigned int size) = 0;
};

class StaticVertexBuffer : public VertexBuffer
{
private:
	GLuint handle;
public:
	StaticVertexBuffer(const void* vertices, unsigned int size);
	~StaticVertexBuffer();
	void bind() const override;
	void unbind() const override;
	void subData(void* vertices, unsigned int size) override;
};

class DynamicVertexBuffer : public VertexBuffer
{
private:
	GLuint handle;
public:
	DynamicVertexBuffer(const void* vertices, unsigned int size);
	~DynamicVertexBuffer();
	void bind() const override;
	void unbind() const override;
	void subData(void* vertices, unsigned int size) override;
};

class IndexBuffer
{
private:
	GLuint handle;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* indices, unsigned int size);
	~IndexBuffer();
	void bind() const;
	void unbind() const;
	void subData(void* vertices, unsigned int size);
	unsigned int getCount() const { return count; }
};


#endif
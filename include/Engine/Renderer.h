#ifndef ENGINE_RENDERER
#define ENGINE_RENDERER

#include <GL/glew.h>
#include <Engine/Shader.h>


class VertexArray;

enum PrimitiveType
{
	Line,
	Triangle
};

class Renderer
{
private:
	static GLenum toGlPrimitive(PrimitiveType type); 
public:
	static void init();
	static void clear();
	static void DrawIndexed(const VertexArray& VA, const Shader& shader, PrimitiveType type);
	static void DrawInstanced(const VertexArray& VA, const Shader& shader, PrimitiveType type, int count);
};

#endif
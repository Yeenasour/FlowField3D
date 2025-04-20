#ifndef ENGINE_GRAPHICSCONTEXT
#define ENGINE_GRAPHICSCONTEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window;

class GraphicsContext
{
private:
	GLFWwindow* windowHandle;
public:
	GraphicsContext(GLFWwindow* handle);
	~GraphicsContext();
	void init();
	void swapBuffers();
	void setViewport(int width, int height);
};


#endif
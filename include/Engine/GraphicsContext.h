#ifndef FLOWFIELD_GRAPHICSCONTEXT
#define FLOWFIELD_GRAPHICSCONTEXT

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
};


#endif
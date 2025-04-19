#include <GraphicsContext.h>
#include <Window.h>
#include <iostream>

//GraphicsContext(Window* handle);
//~GraphicsContext();
//void init();
//void swapBuffers();

GraphicsContext::GraphicsContext(GLFWwindow* handle)
	: windowHandle(handle)
{

}

GraphicsContext::~GraphicsContext()
{

}

void GraphicsContext::init()
{
	glfwMakeContextCurrent(windowHandle);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize" << "\n";
	}
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
}

void GraphicsContext::swapBuffers()
{
	glfwSwapBuffers(windowHandle);
}
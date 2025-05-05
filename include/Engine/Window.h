#ifndef ENGINE_WINDOW
#define ENGINE_WINDOW

#include <string>
#include <GLFW/glfw3.h>
#include <functional>
#include <Engine/Event.h>


class GraphicsContext;

struct WindowProperties
{
	int width, height;
	std::string name;
};

class Window
{
private:
	GLFWwindow* window;
	GraphicsContext* context;
	struct WindowData
	{
		std::string name;
		int width, height;
		std::function<void(Event&)> callback;
	};
	WindowData data;
	
	void init(const WindowProperties& props);
	void initCallbacks();
	static int keyMapping(int key);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
public:
	Window(const WindowProperties& props);
	~Window();
	void update();
	void destroy();
	void setVSync(bool enable);
	void setEventCallback(std::function<void(Event&)> eventCallback);
	void setViewport(int width, int height) const;
	GLFWwindow* getWindow() const { return window; }
	std::string getName() const { return data.name; }
	int getWidth() const { return data.width; }
	int getHeight() const { return data.height; }
	float getAspectRatio() const { return (float)data.width/(float)data.height; }
	double getTime() const { return glfwGetTime(); }
};

#endif
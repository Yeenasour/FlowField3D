#ifndef FLOWFIELD_WINDOW
#define FLOWFIELD_WINDOW

#include <string>
#include <GLFW/glfw3.h>
#include <functional>


class Event;

struct WindowProperties
{
	int width, height;
	std::string name;
};

class Window
{
private:
	GLFWwindow* window;
	struct WindowData
	{
		std::string name;
		int width, height;
		std::function<void(const Event&)> callback;
	};
	WindowData data;
	
	void init(const WindowProperties& props);
	void initCallbacks();
public:
	Window(const WindowProperties& props);
	~Window();
	void update();
	void destroy();
	void setVSync(bool enable);
	void setEventCallback(std::function<void(const Event&)> eventCallback);
	GLFWwindow* getWindow() const { return window; }
	std::string getName() const { return data.name; }
	int getWidth() const { return data.width; }
	int getHeight() const { return data.height; }
};

#endif
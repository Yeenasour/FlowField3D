#include <Window.h>
#include <stdexcept>
#include <Event.h>

Window::Window(const WindowProperties& props)
{
	init(props);
}

Window::~Window()
{

}

void Window::init(const WindowProperties& props)
{
	data.width = props.width;
	data.height = props.height;
	data.name = props.name;

	// Only supports one window
	if (!glfwInit()) {
		throw std::runtime_error("GLFW failed to initialize");
	}

	this->window = glfwCreateWindow(
		data.width,
		data.height,
		data.name.c_str(),
		NULL, NULL
	);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowUserPointer(window, &data);
	setVSync(true);

	initCallbacks();
}

void Window::initCallbacks()
{
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;

		WindowResizeEvent e(width, height);
		data.callback(e);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent e(button);
				data.callback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent e(button);
				data.callback(e);
				break;
			}
			default:
				break;
		}
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMoveEvent e(xPos, yPos);
		data.callback(e);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent e(xOffset, yOffset);
		data.callback(e);
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent e(key);
				data.callback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent e(key);
				data.callback(e);
				break;
			}
			default:
				break;
		}
	});
}

void Window::update()
{
	glfwPollEvents();
	// TODO frame-rate limiting
	glfwSwapBuffers(this->window);
}

void Window::destroy()
{
	glfwDestroyWindow(this->window);
}

void Window::setVSync(bool enable)
{
	if (enable)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}
}

void Window::setEventCallback(std::function<void(const Event&)> eventCallback)
{
	data.callback = eventCallback;
}
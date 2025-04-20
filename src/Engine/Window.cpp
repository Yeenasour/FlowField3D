#include <Engine/GraphicsContext.h>
#include <Engine/Window.h>
#include <stdexcept>


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

	context = new GraphicsContext(window);
	context->init();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowUserPointer(window, &data);
	setVSync(true);

	initCallbacks();
}

void Window::initCallbacks()
{
	glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent e;
		data.callback(e);
	});

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

	glfwSetKeyCallback(window, keyCallback);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	int mappedKey = keyMapping(key);
	switch (action)
	{
		case GLFW_PRESS:
		{
			KeyPressedEvent e(mappedKey);
			data.callback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent e(mappedKey);
			data.callback(e);
			break;
		}
		default:
			break;
	}
}

void Window::update()
{
	glfwPollEvents();
	// TODO frame-rate limiting
	context->swapBuffers();
}

void Window::destroy()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
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

void Window::setEventCallback(std::function<void(Event&)> eventCallback)
{
	data.callback = eventCallback;
}

void Window::setViewport(int width, int height) const 
{
	context->setViewport(width, height);
}

int Window::keyMapping(int key)
{
	switch (key)
	{
		case GLFW_KEY_W: return 0;
		case GLFW_KEY_A: return 1;
		case GLFW_KEY_S: return 2;
		case GLFW_KEY_D: return 3;
		case GLFW_KEY_SPACE: return 4;
		case GLFW_KEY_LEFT_CONTROL: return 5;
		default: return -1;
	}
}
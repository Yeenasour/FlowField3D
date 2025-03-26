#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <FileUtils.h>
#include <Shader.h>
#include <Camera.h>
#include <Axes.h>
#include <VectorFieldRenderer.h>


struct ApplicationData
{
	Camera camera;
	double lastMouseX, lastMouseY;
	bool mouseHeld = false;
};


static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	ApplicationData *ad = (ApplicationData*)glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);

    ad->camera.setAspectRatio((float)width/(float)height);
}

static void mouseButtonHandler(GLFWwindow *window, int button, int action, int mods)
{
	ApplicationData *ad = (ApplicationData*)glfwGetWindowUserPointer(window);
	ad->mouseHeld = (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT);
	if (ad->mouseHeld)
	{
		glfwGetCursorPos(window, &ad->lastMouseX, &ad->lastMouseY);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

static void mouseMoveHandler(GLFWwindow *window, double xPos, double yPos)
{
	ApplicationData *ad = (ApplicationData*)glfwGetWindowUserPointer(window);
	if (ad->mouseHeld)
	{
		constexpr float sensitivity = 0.005;
		float dx = (ad->lastMouseX - xPos) * sensitivity;
		float dy = (yPos - ad->lastMouseY) * sensitivity;
		ad->camera.rotate(dy, dx);
		ad->lastMouseX = xPos;
		ad->lastMouseY = yPos;
	}
}

static void keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "GLFW failed to initialize" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(1080, 1080, "FlowField3D", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetKeyCallback(window, keyHandler);

	Camera camera = Camera(
		glm::vec3(2.5f, 2.5f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		{glm::radians(45.0f),(float)1080/(float)1080, 0.1f, 100.0f}
	);

	ApplicationData appData = {camera};

	glfwSetWindowUserPointer(window, &appData);

	std::cout << glGetString(GL_VERSION) << std::endl;

	Axes axes = Axes(1.0f);

	Shader program = Shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");
	program.use();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	int maxFPS;
	
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
		maxFPS = mode->refreshRate;
	}

	int targetFPS = maxFPS;
	float targetFrameTime = 1.0f / targetFPS;
	double lastDrawTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();

		double currentTime = glfwGetTime();
		double deltaDrawTime = currentTime - lastDrawTime;

		if (deltaDrawTime > targetFrameTime) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			axes.Draw(appData.camera, program);			

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			lastDrawTime = currentTime;
		}
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
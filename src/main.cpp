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
#include <Axes.h>
#include <VectorFieldRenderer.h>
#include <VectorField.h>
#include <OrbitalCamera.h>
#include <FreeCamera.h>
#include <ParticleSystem.h>


struct ApplicationData
{
	Camera* camera;
	VectorField field;
	double lastMouseX, lastMouseY;
	bool mouseHeld = false;
	bool newField = false;
	bool keyStatus[6];
};


static int keyToIndex(int key)
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

static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	ApplicationData *ad = (ApplicationData*)glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);

    ad->camera->setAspectRatio((float)width/(float)height);
}

static void mouseButtonHandler(GLFWwindow *window, int button, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse) return;
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
		constexpr float sensitivity = 0.005f;
		float dx = (ad->lastMouseX - xPos) * sensitivity;
		float dy = (yPos - ad->lastMouseY) * sensitivity;
		ad->camera->rotate(dy, dx);
		ad->lastMouseX = xPos;
		ad->lastMouseY = yPos;
	}
}

static void scrollHandler(GLFWwindow *window, double xOffset, double yOffset)
{
	// <0 out, >0 in
	ApplicationData * ad = (ApplicationData*)glfwGetWindowUserPointer(window);
	constexpr float zoomFactor = 0.1f;
	yOffset = (-yOffset * zoomFactor) + 1;
	ad->camera->zoom(yOffset);
}

static void keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	ApplicationData *ad = (ApplicationData*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}
	int keyIndex = keyToIndex(key);
	if (keyIndex == -1 || action == GLFW_REPEAT) return;
	ad->keyStatus[keyIndex] = (action == GLFW_PRESS);
}

static void update(ApplicationData* ad, double dt)
{
	constexpr static float movementSpeed = 5.0f;
	if (ImGui::IsAnyItemActive()) return;
	double dx = (ad->keyStatus[3] - ad->keyStatus[1]);
	double dy = (ad->keyStatus[4] - ad->keyStatus[5]);
	double dz = (ad->keyStatus[2] - ad->keyStatus[0]);
	glm::vec3 delta = glm::vec3(dx, dy, dz) * movementSpeed * (float)dt;
	ad->camera->move(delta);
}

static int editCallback(ImGuiInputTextCallbackData* data)
{
	ApplicationData *ad = (ApplicationData*)glfwGetWindowUserPointer((GLFWwindow*)data->UserData);
	std::string equation(data->Buf);
	ad->field.setField(equation);
	ad->newField = true;
	return 0;
}

int main()
{
	GLFWwindow* window;

	static char buffer[256] = "x,x,x";

	if (!glfwInit()) {
		std::cout << "GLFW failed to initialize" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(1080, 1080, "FlowField3D", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetScrollCallback(window, scrollHandler);
	glfwSetKeyCallback(window, keyHandler);

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

	glm::vec3 camStartPos = glm::vec3(2.5f, 2.5f, 15.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

	OrbitalCamera orbitCam = OrbitalCamera(
		camStartPos,
		target,
		{glm::radians(45.0f), 1080.f/1080.f, 0.1f, 100.0f}
	);

	FreeCamera freeCam = FreeCamera(
		camStartPos,
		glm::normalize(target - camStartPos),
		{glm::radians(45.0f), 1080.f/1080.f, 0.1f, 100.0f}
	);

	ApplicationData appData = {&freeCam, VectorField("x,x,x")};
	for (int i = 0; i < 6; i++)
	{
		appData.keyStatus[i] = false;
	}
	

	glfwSetWindowUserPointer(window, &appData);

	std::cout << glGetString(GL_VERSION) << std::endl;

	Axes axes = Axes(5.0f);

	Shader program = Shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");
	program.use();

	/* TODO
		Create application-class
		Improve particle shader.
		Grabbing the window freezes the application, causes particles to sync up in lifetime
	*/

	VectorFieldRenderer fieldRenderer = VectorFieldRenderer(appData.field, 6, 10, 5);

	ParticleSystem particles = ParticleSystem(100);
	Shader particleProgram = Shader("../src/shaders/particle.vert", "../src/shaders/particle.frag");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int maxFPS;
	
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
		maxFPS = mode->refreshRate;
	}

	int targetFPS = maxFPS;
	float targetFrameTime = 1.0f / targetFPS;
	double lastDrawTime = glfwGetTime();
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		double deltaDrawTime = currentTime - lastDrawTime;

		update(&appData, deltaTime);
		particles.update(appData.field, deltaTime);

		if (deltaDrawTime > targetFrameTime) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			ImGui::SetNextWindowSize(ImVec2(300, 100));
			ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoResize);

			ImGui::BeginGroup();
			ImGui::Text("Camera-Type");
			if (ImGui::Button("Free-Cam")) appData.camera = &freeCam;
			ImGui::SameLine();
			if (ImGui::Button("Orbital-Cam")) appData.camera = &orbitCam;
			ImGui::EndGroup();
			
			ImGui::InputText("Equation", buffer, 256, ImGuiInputTextFlags_CallbackEdit, editCallback, (void*)window);

			ImGui::End();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			axes.Draw(*appData.camera, program);

			if (appData.newField) 
			{
				fieldRenderer.updateBuffers();
				appData.newField = false;
			}
			
			fieldRenderer.Draw(*appData.camera, program);

			glDepthMask(GL_FALSE);
			particles.Draw(*appData.camera, particleProgram);
			glDepthMask(GL_TRUE);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			lastDrawTime = currentTime;
		}
		lastTime = currentTime;
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
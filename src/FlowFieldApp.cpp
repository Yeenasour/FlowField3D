#include <FlowFieldApp.h>
#include <Engine/Renderer.h>
#include <Engine/Window.h>
#include <Engine/Shader.h>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <Engine/FreeCamera.h>
#include <Engine/OrbitalCamera.h>
#include <Axes.h>
#include <ParticleSystem.h>
#include <VectorFieldRenderer.h>

#define BIND_CALLBACK(e) std::bind(&e, this, std::placeholders::_1)

/*
	TODO ideas
	Add constant that you can scale with a slider
	Add t variable
	Add a function for absolute value
	Add scaling factor for coordinate system, apply for xyz before each eval.
	Add point-particle-spawner to follow particles from a specific point.
*/

void FlowFieldApp::run()
{
	Window window = Window({1080, 1080, "FlowField3D"});
	window.setEventCallback(BIND_CALLBACK(FlowFieldApp::onEvent));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init();

	data = new ApplicationData();

	glm::vec3 camStartPos = glm::vec3(2.5f, 2.5f, 15.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

	OrbitalCamera orbitCam = OrbitalCamera(
		camStartPos,
		target,
		{glm::radians(45.0f), window.getAspectRatio(), 0.1f, 100.0f}
	);

	FreeCamera freeCam = FreeCamera(
		camStartPos,
		glm::normalize(target - camStartPos),
		{glm::radians(45.0f), (float)window.getWidth()/(float)window.getHeight(), 0.1f, 100.0f}
	);

	VectorField field("-y-x,x-y,-z");

	data->window = &window;
	data->camera = &freeCam;
	data->field = &field;
	for (int i = 0; i < 6; i++)
	{
		data->keyStatus[i] = false;
	}
	
	Axes axes = Axes(5.0f);

	Shader program = Shader("../src/shaders/simple.vert", "../src/shaders/simple.frag");
	program.use();

	VectorFieldRenderer fieldRenderer(*data->field, 10, 5);
	Shader vectorProgram = Shader("../src/shaders/vector.vert", "../src/shaders/vector.frag");

	ParticleSystem particles = ParticleSystem(data->particleCount, data->partileLifetime);
	Shader particleProgram = Shader("../src/shaders/particle.vert", "../src/shaders/particle.frag");

	Renderer::init();

	double lastTime = window.getTime();
	double accumulator = 0.0;
	constexpr double simulationTimestep = 1.0f / 60.0f;

	while (running)
	{
		double currentTime = window.getTime();
		double deltaTime = currentTime - lastTime;

		accumulator += deltaTime;

		if (accumulator > simulationTimestep)
		{
			update(simulationTimestep);
			particles.update(*data->field, simulationTimestep);
			accumulator = 0.0;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::SetNextWindowSize(ImVec2(300, 250));
		ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoResize);

		ImGui::Text("DeltaTime: %.2fms", deltaTime * 1000);
		ImGui::BeginGroup();
		ImGui::Text("Camera-Type");
		if (ImGui::Button("Free-Cam"))
		{
			freeCam.setAspectRatio(window.getAspectRatio());
			data->camera = &freeCam;
		}
		ImGui::SameLine();
		if (ImGui::Button("Orbital-Cam"))
		{
			orbitCam.setAspectRatio(window.getAspectRatio());
			data->camera = &orbitCam;
		}
		ImGui::EndGroup();
		
		ImGui::InputText("Equation", data->expressionBuffer, 256, ImGuiInputTextFlags_CallbackEdit, FlowFieldApp::editCallback, (void*)data);

		ImGui::BeginGroup();
		ImGui::Text("Particle Control");
		if (ImGui::SliderInt("Particle Count", &data->particleCount, 1, 1000)) particles.setParticleCount(data->particleCount);
		if (ImGui::SliderFloat("Particle Lifetime", &data->partileLifetime, 0.5, 10.0)) particles.setLifetime(data->partileLifetime);
		ImGui::Button("Reset particles");

		ImGui::EndGroup();

		ImGui::End();

		Renderer::clear();

		glm::mat4 VP = data->camera->getViewProjectionMatrix();
		glm::mat4 V = data->camera->getViewMatrix();
		glm::mat4 P = data->camera->getProjectionMatrix();
		glm::mat4 M = axes.getModelMatrix();

		particleProgram.use();

		particleProgram.setUniform1f("particleSize", 0.1f);
		particleProgram.setUniform1f("maxLifetime", data->partileLifetime);
		particleProgram.setUniform4fv("viewMatrix", &V[0][0]);
		particleProgram.setUniform4fv("projectionMatrix", &P[0][0]);

		Renderer::DrawInstanced(particles, particleProgram, PrimitiveType::Triangle, particles.getCount());

		program.use();

		program.setUniform4fv("modelViewProjectionMatrix", &(VP * M)[0][0]);

		Renderer::DrawIndexed(axes, program, PrimitiveType::Line);

		if (data->newField) 
		{
			fieldRenderer.updateBuffers();
			data->newField = false;
		}

		vectorProgram.use();

		M = fieldRenderer.getModelMatrix();
		glm::vec3 camPos = data->camera->getPosition();

		vectorProgram.setUniform1f("vectorScale", 0.25f);
		vectorProgram.setUniform3f("cameraPos", camPos.x, camPos.y, camPos.z);
		vectorProgram.setUniform4fv("modelMatrix", &(M)[0][0]);
		vectorProgram.setUniform4fv("viewProjection", &(VP)[0][0]);

		Renderer::DrawInstanced(fieldRenderer, vectorProgram, PrimitiveType::Triangle, 1000);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();

		lastTime = currentTime;
	}
}

void FlowFieldApp::update(double dt)
{
	constexpr static float movementSpeed = 5.0f;
	if (ImGui::IsAnyItemActive()) return;
	double dx = (data->keyStatus[3] - data->keyStatus[1]);
	double dy = (data->keyStatus[4] - data->keyStatus[5]);
	double dz = (data->keyStatus[2] - data->keyStatus[0]);
	glm::vec3 delta = glm::vec3(dx, dy, dz) * movementSpeed * (float)dt;
	data->camera->move(delta);
}

void FlowFieldApp::shutdown()
{
	data->window->destroy();
	delete data;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int FlowFieldApp::editCallback(ImGuiInputTextCallbackData* data)
{
	ApplicationData *ad = (ApplicationData*)data->UserData;
	std::string equation(data->Buf);
	ad->field->setField(equation);
	ad->newField = true;
	return 0;
}

void FlowFieldApp::onEvent(Event& event)
{
	EventHandler handler(event);
	handler.handleEvent<WindowCloseEvent>(BIND_CALLBACK(FlowFieldApp::onWindowClose));
	handler.handleEvent<WindowResizeEvent>(BIND_CALLBACK(FlowFieldApp::onWindowResize));
	handler.handleEvent<MouseButtonPressedEvent>(BIND_CALLBACK(FlowFieldApp::onMouseButtonPressed));
	handler.handleEvent<MouseButtonReleasedEvent>(BIND_CALLBACK(FlowFieldApp::onMouseButtonReleased));
	handler.handleEvent<MouseMoveEvent>(BIND_CALLBACK(FlowFieldApp::onMouseMove));
	handler.handleEvent<MouseScrolledEvent>(BIND_CALLBACK(FlowFieldApp::onMouseScroll));
	handler.handleEvent<KeyPressedEvent>(BIND_CALLBACK(FlowFieldApp::onKeyPressed));
	handler.handleEvent<KeyReleasedEvent>(BIND_CALLBACK(FlowFieldApp::onKeyReleased));
}

void FlowFieldApp::onWindowClose(WindowCloseEvent& event)
{
	running = false;
}

void FlowFieldApp::onWindowResize(WindowResizeEvent& event)
{
	int w = event.getWidth();
	int h = event.getHeight();
	data->window->setViewport(w, h);
	data->camera->setAspectRatio((float)w/(float)h);
}

void FlowFieldApp::onMouseButtonPressed(MouseButtonPressedEvent& event)
{
	if (ImGui::GetIO().WantCaptureMouse) return;
	data->mouseHeld = true;
	// Change input mode
}

void FlowFieldApp::onMouseButtonReleased(MouseButtonReleasedEvent& event)
{
	data->mouseHeld = false;
	// Change input mode
}

void FlowFieldApp::onMouseMove(MouseMoveEvent& event)
{
	if (data->mouseHeld)
	{
		constexpr float sensitivity = 0.005f;
		float dx = (data->lastMouseX - event.getXPos()) * sensitivity;
		float dy = (event.getYPos() - data->lastMouseY) * sensitivity;
		data->camera->rotate(dy, dx);
	}
	data->lastMouseX = event.getXPos();
	data->lastMouseY = event.getYPos();
}

void FlowFieldApp::onMouseScroll(MouseScrolledEvent& event)
{
	constexpr float zoomFactor = 0.1f;
	float yOffset = (-event.getYOffset() * zoomFactor) + 1;
	data->camera->zoom(yOffset);
}

void FlowFieldApp::onKeyPressed(KeyPressedEvent& event)
{
	int key = event.key();
	if (key == -1) return;
	data->keyStatus[key] = true;
}

void FlowFieldApp::onKeyReleased(KeyReleasedEvent& event)
{
	int key = event.key();
	if (key == -1) return;
	data->keyStatus[key] = false;
}

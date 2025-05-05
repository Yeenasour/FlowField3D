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
#include <iostream>

#define BIND_CALLBACK(e) std::bind(&e, this, std::placeholders::_1)


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
		{glm::radians(45.0f), 1080.f/1080.f, 0.1f, 100.0f}
	);

	FreeCamera freeCam = FreeCamera(
		camStartPos,
		glm::normalize(target - camStartPos),
		{glm::radians(45.0f), 1080.f/1080.f, 0.1f, 100.0f}
	);

	VectorField field = VectorField("-y-x,x-y,-z");

	data->window = &window;
	data->camera = &freeCam;
	data->field = &field;
	for (int i = 0; i < 6; i++)
	{
		data->keyStatus[i] = false;
	}
	
	Axes axes = Axes(5.0f);

	Shader program = Shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");
	program.use();

	VectorFieldRenderer fieldRenderer = VectorFieldRenderer(*data->field, 6, 10, 5);

	ParticleSystem particles = ParticleSystem(100);
	Shader particleProgram = Shader("../src/shaders/particle.vert", "../src/shaders/particle.frag");

	Renderer::init();

	double lastTime = window.getTime();

	while (running)
	{
		double currentTime = window.getTime();
		double deltaTime = currentTime - lastTime;

		update(deltaTime);
		particles.update(*data->field, deltaTime);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::SetNextWindowSize(ImVec2(300, 100));
		ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoResize);

		ImGui::BeginGroup();
		ImGui::Text("Camera-Type");
		if (ImGui::Button("Free-Cam")) data->camera = &freeCam;
		ImGui::SameLine();
		if (ImGui::Button("Orbital-Cam")) data->camera = &orbitCam;
		ImGui::EndGroup();
		
		ImGui::InputText("Equation", data->expressionBuffer, 256, ImGuiInputTextFlags_CallbackEdit, FlowFieldApp::editCallback, (void*)data);

		ImGui::End();

		Renderer::clear();

		program.use();
		glm::mat4 VP = data->camera->getViewProjectionMatrix();
		glm::mat4 V = data->camera->getViewMatrix();
		glm::mat4 P = data->camera->getProjectionMatrix();
		glm::mat4 M = axes.getModelMatrix();

		program.setUniform4fv("modelViewProjectionMatrix", &(VP * M)[0][0]);

		Renderer::DrawIndexed(axes, program, PrimitiveType::Line);

		if (data->newField) 
		{
			fieldRenderer.updateBuffers();
			data->newField = false;
		}
		
		M = fieldRenderer.getModelMatrix();

		program.setUniform4fv("modelViewProjectionMatrix", &(VP * M)[0][0]);

		Renderer::DrawIndexed(fieldRenderer, program, PrimitiveType::Line);

		particleProgram.use();

		M = particles.getModelMatrix();

		particleProgram.setUniform4fv("modelViewProjectionMatrix", &(VP * M)[0][0]);
		particleProgram.setUniform1f("particleSize", 0.1f);
		particleProgram.setUniform4fv("viewMatrix", &V[0][0]);
		particleProgram.setUniform4fv("projectionMatrix", &P[0][0]);

		Renderer::DrawInstanced(particles, particleProgram, PrimitiveType::Triangle, particles.getCount());

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
	std::cout << event.toString() << std::endl;
	handler.handleEvent<WindowCloseEvent>(BIND_CALLBACK(FlowFieldApp::onWindowClose));
	handler.handleEvent<WindowResizeEvent>(BIND_CALLBACK(FlowFieldApp::onWindowResize));
	handler.handleEvent<MouseButtonPressedEvent>(BIND_CALLBACK(FlowFieldApp::onMouseButtonPressed));
	handler.handleEvent<MouseButtonReleasedEvent>(BIND_CALLBACK(FlowFieldApp::onMouseButtonReleased));
	handler.handleEvent<MouseMoveEvent>(BIND_CALLBACK(FlowFieldApp::onMouseMove));
	handler.handleEvent<KeyPressedEvent>(BIND_CALLBACK(FlowFieldApp::onKeyPressed));
	handler.handleEvent<KeyReleasedEvent>(BIND_CALLBACK(FlowFieldApp::onKeyReleased));
	// TODO scroll
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

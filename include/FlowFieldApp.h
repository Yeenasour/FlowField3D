#ifndef FLOWFIELD_APP
#define FLOWFIELD_APP

#include <Engine/Application.h>
#include <Engine/Event.h>
#include <VectorField.h>


class Camera;
class Window;
struct ImGuiInputTextCallbackData;

struct ApplicationData
{
	char expressionBuffer[256] = "-y-x,x-y,-z";
	Window* window;
	Camera* camera;
	VectorField* field;
	float lastMouseX, lastMouseY;
	int particleCount = 100;
	float particleLifetime = 2.0f;
	float particleInertia = 0.5f;
	float simulationConstant = 1.0f;
	float coordinateScale = 1.0f;
	bool mouseHeld = false;
	bool newField = false;
	bool keyStatus[6];
	bool renderTargets[3];
};

class FlowFieldApp : public Application
{
private:
	ApplicationData* data;
	bool running = true;
	void onEvent(Event& event);
	void onWindowClose(WindowCloseEvent& event);
	void onWindowResize(WindowResizeEvent& event);
	void onMouseButtonPressed(MouseButtonPressedEvent& event);
	void onMouseButtonReleased(MouseButtonReleasedEvent& event);
	void onMouseMove(MouseMoveEvent& event);
	void onMouseScroll(MouseScrolledEvent& event);
	void onKeyPressed(KeyPressedEvent& event);
	void onKeyReleased(KeyReleasedEvent& event);
public:
	FlowFieldApp() = default;
	~FlowFieldApp() = default;
	void run() override;
	void update(double dt) override;
	void shutdown() override;
	static int editCallback(ImGuiInputTextCallbackData* data);
};


#endif
#include <FlowFieldApp.h>

int main()
{
	// TODO normalize field vectors, visualize length by the color
	// TODO understand how binding a vertexarray affects the global state
	FlowFieldApp app = FlowFieldApp();
	app.run();
	app.shutdown();
	return 0;
}
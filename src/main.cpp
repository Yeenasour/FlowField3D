#include <FlowFieldApp.h>

int main()
{
	// TODO normalize field vectors, visualize length by the color
	FlowFieldApp app = FlowFieldApp();
	app.run();
	app.shutdown();
	return 0;
}
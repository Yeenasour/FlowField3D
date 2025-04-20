#ifndef ENGINE_APPLICATION
#define ENGINE_APPLICATION


class Application
{
public:
	virtual ~Application() {};
	virtual void run() = 0;
	virtual void update(double dt) = 0;
	virtual void shutdown() = 0;
};

#endif
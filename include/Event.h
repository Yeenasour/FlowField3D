#ifndef FLOWFIELD_EVENT
#define FLOWFIELD_EVENT

#include <functional>
#include <sstream>


enum EventType {
	None = 0,
	WindowClose,
	WindowResize,
	KeyPressed,
	KeyRepeat,
	KeyReleased,
	MousePressed,
	MouseReleased,
	MouseMoved,
	MouseScrolled
};


class Event;

class EventHandler
{
private:
	Event& e;
public:
	EventHandler(Event& e)
		: e(e) {}

	template<typename T>
	void handleEvent(std::function<void(const T&)> eventCallback)
    {
        if (typeid(T) == typeid(e))
		{
            eventCallback(static_cast<const T&>(e));
			e.handeled = true;
        }
    }
};


class Event
{
	friend EventHandler;
protected:
	bool handeled = false;
public:
	virtual ~Event() = default;
	virtual EventType getEventType() = 0;
	virtual std::string toString() = 0;
};


class WindowResizeEvent : public Event
{
protected:
	int width, height;
public:
	WindowResizeEvent(int w, int h)
		: width(w), height(h) {}
	~WindowResizeEvent() {}
	EventType getEventType() override { return EventType::WindowResize; }
	std::string toString() override 
	{
		std::ostringstream os;
		os << "WindowResizeEvent - width: " << width << ", height: " << height;
		return os.str();
	}
	int getWidth() { return width; }
	int getHeight() { return height; }
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}
	~WindowCloseEvent() {}
	EventType getEventType() override { return EventType::WindowClose; }
	std::string toString() override
	{
		std::ostringstream os;
		os << "WindowCloseEvent";
		return os.str();
	}
};


class KeyEvent : public Event
{
protected:
	int keyCode;
public:
	KeyEvent(int key)
		: keyCode(key) {}
	virtual ~KeyEvent() = default;
	int key() const { return keyCode; }
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int key)
		: KeyEvent(key) {}
	~KeyPressedEvent() {}
	EventType getEventType() override { return EventType::KeyPressed; }
	std::string toString() override
	{
		std::ostringstream os;
		os << "KeyPressedEvent - key: " << keyCode;
		return os.str();
	}
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int key)
		: KeyEvent(key) {}
	~KeyReleasedEvent() {}
	EventType getEventType() override { return EventType::KeyReleased; }
	std::string toString() override
	{
		std::ostringstream os;
		os << "KeyReleasedEvent - key: " << keyCode;
		return os.str();
	}
};


class MouseScrolledEvent : public Event
{
protected:
	float xOffset, yOffset;
public:
	MouseScrolledEvent(float xOffset, float yOffset)
		: xOffset(xOffset), yOffset(yOffset) {}
	~MouseScrolledEvent() {}
	EventType getEventType() override { return EventType::MouseScrolled; }
	std::string toString() override
	{
		std::ostringstream os;
		os << "MouseScrolledEvent - xOffset: " << xOffset << ", yOffset: " << yOffset;
		return os.str();
	}
	float getXOffset() { return xOffset; }
	float getYOffset() { return yOffset; }
};

class MouseMoveEvent : public Event
{
protected:
	float xPos, yPos;
public:
	MouseMoveEvent(float xPos, float yPos)
		: xPos(xPos), yPos(yPos) {}
	~MouseMoveEvent() {}
	EventType getEventType() override { return EventType::MouseMoved; }
	std::string toString() override
	{
		std::ostringstream os;
		os << "MouseMovedEvent - xPos: " << xPos << ", yPos: " << yPos;
		return os.str();
	}
	float getXPos() { return xPos; }
	float getYPos() { return yPos; }
};

class MouseButtonEvent : public Event
{
protected:
	int button;
public:
	MouseButtonEvent(int button)
		: button(button) {}
	virtual ~MouseButtonEvent() = default;
	int getButton() { return button; }
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button)
		: MouseButtonEvent(button) {}
	~MouseButtonPressedEvent() {}
	EventType getEventType() override { return EventType::MousePressed; }
	std::string toString() override
	{
		std::ostringstream os;
		os << "MouseButtonPressedEvent - button: " << button;
		return os.str();
	}
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button)
		: MouseButtonEvent(button) {}
	~MouseButtonReleasedEvent() {}
	EventType getEventType() override { return EventType::MouseReleased; }
	std::string toString() override
	{
		std::ostringstream os;
		os << "MouseButtonReleasedEvent - button: " << button;
		return os.str();
	}
};

#endif
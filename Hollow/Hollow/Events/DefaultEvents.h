#pragma once

#include "Event.h"
#include "Hollow/Input/KeyCodes.h"

namespace Hollow {

	class ButtonPressedEvent : public Event<ButtonPressedEvent>
	{
	public:
		ButtonPressedEvent(eKeyCodes button) : button(button) {}
		eKeyCodes button;
	};

	class ButtonReleasedEvent : public Event<ButtonReleasedEvent>
	{
	public:
		ButtonReleasedEvent(eKeyCodes button) : button(button) {}
		eKeyCodes button;
	};

	class WindowCreateEvent : public Event<WindowCreateEvent>
	{
	public:
		WindowCreateEvent(const char* message) : message(message) {}

		const char* message;
	};

	class MouseMoveEvent : public Event<MouseMoveEvent>
	{
	public:
		MouseMoveEvent(int x, int y) : x(x), y(y) {}
		int x;
		int y;
	};

	class WindowResizeEvent : public Event<WindowResizeEvent>
	{
	public:
		WindowResizeEvent(int width, int height) : width(width), height(height) {}
		int width;
		int height;
	};
}
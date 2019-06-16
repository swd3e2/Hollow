#pragma once
#include "Event.h"

namespace Hollow {
	class WindowCreateEvent : public Event<WindowCreateEvent>
	{
	public:
		WindowCreateEvent(const char* message) : message(message)
		{}

		const char* message;
	};
}

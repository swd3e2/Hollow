#pragma once
#include "Event.h"

class WindowCreateEvent : public Event<WindowCreateEvent>
{
public:
	WindowCreateEvent(const char * message) : message(message)
	{}

	const char* message;
};
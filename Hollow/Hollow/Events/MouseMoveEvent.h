#pragma once
#include "Event.h"
#include "Hollow/Input/KeyCodes.h"

class MouseMoveEvent : public Event<MouseMoveEvent>
{
public:
	MouseMoveEvent(int x, int y) : x(x), y(y) {}
	int x;
	int y;
};
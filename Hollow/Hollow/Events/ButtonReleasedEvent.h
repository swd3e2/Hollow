#pragma once
#include "Event.h"

class ButtonReleasedEvent : public Event<ButtonReleasedEvent>
{
public:
	ButtonReleasedEvent(char button) : button(button) {}
	char button;
};
#pragma once
#include "Event.h"

class ButtonPressedEvent : public Event<ButtonPressedEvent>
{
public:
	ButtonPressedEvent(char button) : button(button) {}
	char button;
};
#pragma once
#include "Event.h"
#include "Hollow/Input/KeyCodes.h"

class ButtonPressedEvent : public Event<ButtonPressedEvent>
{
public:
	ButtonPressedEvent(eKeyCodes button) : button(button) {}
	eKeyCodes button;
};
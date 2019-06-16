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
}

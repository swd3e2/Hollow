#pragma once
#include "Event.h"
#include "Hollow/Input/KeyCodes.h"

namespace Hollow {
	class ButtonReleasedEvent : public Event<ButtonReleasedEvent>
	{
	public:
		ButtonReleasedEvent(eKeyCodes button) : button(button) {}
		eKeyCodes button;
	};
}

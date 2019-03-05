#pragma once
#include "Event.h"

class ButtonPressEvent : public Event<ButtonPressEvent>
{
public:
	int button;
};
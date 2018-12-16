#pragma once
#include "Events/Event.h"

class MoveEvent : public Hollow::Event<MoveEvent>
{
public:
	MoveEvent() : moved(true), Hollow::Event<MoveEvent>() {}
	bool moved;
};


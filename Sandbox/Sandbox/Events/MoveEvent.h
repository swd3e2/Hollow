#pragma once
#include "Hollow/Events/Event.h"

class MoveEvent : public Hollow::Event<MoveEvent>
{
public:
	MoveEvent() : moved(true), Hollow::Event<MoveEvent>() {}
	bool moved;
};


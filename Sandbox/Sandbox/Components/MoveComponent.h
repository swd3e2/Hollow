#pragma once
#include "Hollow/ECS/Components/Component.h"

class MoveComponent : public Hollow::Component<MoveComponent>
{
public:
	MoveComponent() : move(true) {}
	bool move;
};
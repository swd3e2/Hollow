#pragma once
#include "Hollow/ECS/Entity.h"
#include "Hollow/ECS/Components/PositionComponent.h"

class GameObject : public Hollow::Entity<GameObject>
{
public:
	GameObject() 
	{
	}
};

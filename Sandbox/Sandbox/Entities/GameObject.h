#pragma once
#include "Hollow/ECS/Entities/Entity.h"

class GameObject : public Hollow::Entity<GameObject>
{
public:
	GameObject(std::string stroke) {}
};

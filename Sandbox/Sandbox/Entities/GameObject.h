#pragma once

#include <Hollow/ECS/Entity.h>

class GameObject : public Hollow::Entity<GameObject>
{
public:
	std::string name;
public:
	void SetName(const char* name)
	{
		this->name = name;
	}
};

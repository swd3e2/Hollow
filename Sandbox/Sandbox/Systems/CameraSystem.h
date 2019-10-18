#pragma once

#include "Hollow/ECS/System.h"

class CameraSystem : public Hollow::System<CameraSystem>
{
public:
	CameraSystem()
	{

	}

	virtual void PreUpdate(double dt) override
	{}

	virtual void Update(double dt) override
	{}

	virtual void PostUpdate(double dt) override
	{}
};
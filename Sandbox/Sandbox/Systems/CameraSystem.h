#pragma once

#include "Hollow/ECS/System.h"
#include "Hollow/Graphics/Camera.h"

class CameraSystem : public Hollow::System<CameraSystem>
{
private:
	Hollow::Camera* mCamera;
public:
	CameraSystem()
	{

	}

	void setCamera(Hollow::Camera* camera)
	{
		mCamera = camera;
	}

	virtual void PreUpdate(double dt) override
	{}

	virtual void Update(double dt) override
	{
	
	}

	virtual void PostUpdate(double dt) override
	{}
};
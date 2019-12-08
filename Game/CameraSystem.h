#pragma once

#include "Hollow/ECS/System.h"
#include "Hollow/Graphics/Camera.h"
#include <Hollow/Input/InputManager.h>

class CameraSystem : public Hollow::System<CameraSystem>
{
private:
	Hollow::Camera* mCamera;
	bool updated;

	float bias = 0.000001f;
	float cameraMoveSpeed = 0.015f;
	float cameraMouseMoveSpeed = 0.2f;
public:
	CameraSystem(Hollow::Camera* camera) : 
		mCamera(camera)
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
		updated = false;

		if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_UP) == true) {
			updated = true;
			mCamera->m_Position.y += 1.0f * (float)dt * cameraMoveSpeed;
		} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_DOWN) == true) {
			updated = true;
			mCamera->m_Position.y -= 1.0f * (float)dt * cameraMoveSpeed;
		}

		if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_LEFT) == true) {
			updated = true;
			mCamera->m_Position.x -= 1.0f * (float)dt * cameraMoveSpeed;
		} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_RIGHT) == true) {
			updated = true;
			mCamera->m_Position.x += 1.0f * (float)dt * cameraMoveSpeed;
		}

		if (updated) {
			mCamera->updateViewMatrix();
		}
	}

	virtual void PostUpdate(double dt) override
	{}
};
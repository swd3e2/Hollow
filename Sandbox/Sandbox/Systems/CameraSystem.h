#pragma once

#include "Hollow/ECS/System.h"
#include "Hollow/Graphics/Camera.h"

class CameraSystem : public Hollow::System<CameraSystem>
{
private:
	Hollow::Camera* mCamera;
	bool updated;

	float bias = 0.000001f;
	float cameraMoveSpeed = 0.015f;
	float cameraMouseMoveSpeed = 0.2f;
public:
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
			mCamera->m_Position += mCamera->forwardVec * (float)dt * cameraMoveSpeed;
			mCamera->updateViewMatrix();
		} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_DOWN) == true) {
			updated = true;
			mCamera->m_Position -= mCamera->forwardVec * (float)dt * cameraMoveSpeed;
			mCamera->updateViewMatrix();
		}

		if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_LEFT) == true) {
			updated = true;
			mCamera->m_Position -= mCamera->rightVec * (float)dt * cameraMoveSpeed;
			mCamera->updateViewMatrix();
		} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_RIGHT) == true) {
			updated = true;
			mCamera->m_Position += mCamera->rightVec * (float)dt * cameraMoveSpeed;
			mCamera->updateViewMatrix();
		}

		if (Hollow::InputManager::GetMouseButtonIsPressed(Hollow::eMouseKeyCodes::MOUSE_RIGHT)) {
			updated = true;
			mCamera->m_Rotation.x -= (float)Hollow::InputManager::my * 0.00045f * (float)dt;
			if (mCamera->m_Rotation.x > Hollow::Math::HALF_PI - bias) {
				mCamera->m_Rotation.x = Hollow::Math::HALF_PI - bias;
			} else if (mCamera->m_Rotation.x < -Hollow::Math::HALF_PI + bias) {
				mCamera->m_Rotation.x = -Hollow::Math::HALF_PI + bias;
			}
			mCamera->m_Rotation.y -= (float)Hollow::InputManager::mx * 0.00045f * (float)dt;
			mCamera->updateViewMatrix();
		}

		if (Hollow::InputManager::GetMouseButtonIsPressed(Hollow::eMouseKeyCodes::MOUSE_MIDDLE)) {
			updated = true;
			if (Hollow::InputManager::lastY < 0) {
				mCamera->m_Position -= mCamera->upVec * (float)dt * cameraMouseMoveSpeed;
			} else if (Hollow::InputManager::lastY > 0) {
				mCamera->m_Position += mCamera->upVec * (float)dt * cameraMouseMoveSpeed;
			}
			if (Hollow::InputManager::lastX > 0) {
				mCamera->m_Position -= mCamera->rightVec * (float)dt * cameraMouseMoveSpeed;
			} else if (Hollow::InputManager::lastX < 0) {
				mCamera->m_Position += mCamera->rightVec * (float)dt * cameraMouseMoveSpeed;
			}
			mCamera->updateViewMatrix();
		}
	}

	virtual void PostUpdate(double dt) override
	{}
};
#include "MyCamera.h"

MyCamera::MyCamera(bool mainCamera)
{
	this->position = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	this->rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	this->mainCamera = mainCamera;
	this->UpdateViewMatrix();
	timer.Start();
}

void MyCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = fovDegrees / 360.0f * XM_2PI;
	this->projectionMatrix = Matrix4::Projection(fovRadians, aspectRatio, nearZ, farZ);
}

const Matrix4& MyCamera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const Matrix4 & MyCamera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

void MyCamera::Update(double dt)
{
	if (mainCamera) {
		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_W) == true)
		{
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_S) == true)
		{
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_A) == true)
		{
			position.x -= 0.1f;
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_D) == true)
		{
			position.x += 0.1f;
		}

		if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_RIGHT))
		{
		}
		UpdateViewMatrix();

	}

	//position.x = sinf(timer.GetMilisecondsElapsed()) * radius;
	//position.z = cosf(timer.GetMilisecondsElapsed()) * radius;

	//UpdateViewMatrix();
}

void MyCamera::UpdateViewMatrix()
{
	camTarget = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4 upVec(0.0f, 1.0f, 0.0f, 0.0f);
	//this->viewMatrix = Matrix4::LookAt(position, camTarget, upVec);
	
	this->viewMatrix = Matrix4::Rotation(0.0f, position.x,0.0f);
}

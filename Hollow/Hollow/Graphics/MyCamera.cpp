#include "MyCamera.h"

MyCamera::MyCamera(bool mainCamera)
{
	position = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	mainCamera = mainCamera;
	UpdateViewMatrix();
}

void MyCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = fovDegrees / 360.0f * XM_2PI;
	projectionMatrix = Matrix4::Projection(fovRadians, aspectRatio, nearZ, farZ);
}

const Matrix4& MyCamera::GetViewMatrix() const
{
	return viewMatrix;
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
			position += forwardVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_S) == true)
		{
			position += backVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_A) == true)
		{
			position += leftVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_D) == true)
		{
			position += rightVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_RIGHT))
		{
			angView.pitch += InputManager::my * 0.06f * dt;
			angView.yaw += InputManager::mx * 0.06f * dt;;

			//rotation.x += InputManager::my * 0.06f;
			rotation.y += InputManager::mx * 0.0006f * dt;
			UpdateViewMatrix();
		}
	}
}

void MyCamera::UpdateViewMatrix()
{
	Vector4 upVec(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 fVec(0.0f, 0.0f, 1.0f, 0.0f);

	Matrix4 rotation = Matrix4::Rotation(this->rotation.x, this->rotation.y, 0.0f);
	camTarget = fVec * rotation;
	camTarget = camTarget + position;

	Matrix4 temp = Matrix4::RotationY(this->rotation.y);
	
	forwardVec = Vector4 (0.0f, 0.0f, 1.0f, 0.0f);
	backVec = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	rightVec = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	leftVec = Vector4(-1.0f, 0.0f, 0.0f, 0.0f);

	viewMatrix = Matrix4::LookAt(position, camTarget, upVec);
}

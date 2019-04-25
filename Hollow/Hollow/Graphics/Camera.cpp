#include "Camera.h"

Camera::Camera(bool mainCamera) : 
	mainCamera(mainCamera)
{
	position = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	mainCamera = mainCamera;
	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = fovDegrees / 360.0f * XM_2PI;
	projectionMatrix = Matrix4::Projection(fovRadians, aspectRatio, nearZ, farZ);
}

const Matrix4& Camera::GetViewMatrix() const
{
	return viewMatrix;
}

const Matrix4 & Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

void Camera::Update(double dt)
{
	if (mainCamera) {
		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_W) == true)
		{
			position += forwardVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_S) == true)
		{
			position -= forwardVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_A) == true)
		{
			position -= rightVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_D) == true)
		{
			position += rightVec * dt * cameraMoveSpeed;
			UpdateViewMatrix();
		}

		if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_RIGHT))
		{
			rotation.x -= InputManager::my * 0.0006f * dt;
			rotation.y -= InputManager::mx * 0.0006f * dt;
			UpdateViewMatrix();
		}
		UpdateViewMatrix();
	}
}

void Camera::UpdateViewMatrix()
{
	Vector4 fVec(0.0f, 0.0f, 1.0f, 0.0f);
	Matrix4 rotation = Matrix4::Rotation(this->rotation.x, this->rotation.y, 0.0f);
	camTarget = Vector4::Normalize(fVec * rotation);
	camTarget = camTarget + position;
	viewMatrix = Matrix4::LookAt(position, camTarget, Vector4(0.0f, 1.0f, 0.0f, 0.0f));

	Matrix4 temp = Matrix4::Rotation(this->rotation.x, this->rotation.y, this->rotation.z);

	forwardVec = Vector4(0.0f, 0.0f, 1.0f, 0.0f) * temp;
	rightVec = Vector4(1.0f, 0.0f, 0.0f, 0.0f) * temp;
}

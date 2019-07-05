#include "Camera.h"

namespace Hollow {
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

	void Camera::SetOrthoValues(float right, float left, float top, float bottom, float nearZ, float farZ)
	{
		projectionMatrix = Matrix4::Orthographic(right, left, top, bottom, nearZ, farZ);
	}

	const Matrix4& Camera::GetViewMatrix() const
	{
		return viewMatrix;
	}

	const Matrix4& Camera::GetProjectionMatrix() const
	{
		return projectionMatrix;
	}

	void Camera::Update(double dt)
	{
		if (mainCamera) {
			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_UP) == true) {
				position += forwardVec * dt * cameraMoveSpeed;
				UpdateViewMatrix();
			}

			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_DOWN) == true) {
				position -= forwardVec * dt * cameraMoveSpeed;
				UpdateViewMatrix();
			}

			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_LEFT) == true) {
				position -= rightVec * dt * cameraMoveSpeed;
				UpdateViewMatrix();
			}

			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_RIGHT) == true) {
				position += rightVec * dt * cameraMoveSpeed;
				UpdateViewMatrix();
			}

			if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_RIGHT)) {
				rotation.x -= InputManager::my * 0.0006 * dt;
				/*if (rotation.x > Math::HALF_PI - bias)
				{
					rotation.x = Math::HALF_PI - bias;
				} else if (rotation.x < -Math::HALF_PI + bias)
				{
					rotation.x = -Math::HALF_PI + bias;
				}*/
				rotation.y -= InputManager::mx * 0.0006 * dt;
				UpdateViewMatrix();
			}
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
}
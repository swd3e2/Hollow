#include "Camera.h"

namespace Hollow {
	Camera::Camera(bool mainCamera) :
		m_IsMainCamera(mainCamera)
	{
		m_Position = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		m_Rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		mainCamera = mainCamera;
		updateViewMatrix();
	}

	void Camera::setProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
	{
		float fovRadians = fovDegrees / 360.0f * XM_2PI;
		m_ProjectionMatrix = Matrix4::projection(fovRadians, aspectRatio, nearZ, farZ);
	}

	void Camera::setOrthoValues(float right, float left, float top, float bottom, float nearZ, float farZ)
	{
		m_ProjectionMatrix = Matrix4::orthographic(right, left, top, bottom, nearZ, farZ);
	}

	void Camera::update(double dt)
	{
		if (m_IsMainCamera) {
			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_UP) == true) {
				m_Position += forwardVec * dt * cameraMoveSpeed;
				updateViewMatrix();
			} else if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_DOWN) == true) {
				m_Position -= forwardVec * dt * cameraMoveSpeed;
				updateViewMatrix();
			}

			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_LEFT) == true) {
				m_Position -= rightVec * dt * cameraMoveSpeed;
				updateViewMatrix();
			} else if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_RIGHT) == true) {
				m_Position += rightVec * dt * cameraMoveSpeed;
				updateViewMatrix();
			}

			if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_RIGHT)) {
				m_Rotation.x -= InputManager::my * 0.00045 * dt;
				if (m_Rotation.x > Math::HALF_PI - bias) {
					m_Rotation.x = Math::HALF_PI - bias;
				} else if (m_Rotation.x < -Math::HALF_PI + bias) {
					m_Rotation.x = -Math::HALF_PI + bias;
				}
				m_Rotation.y -= InputManager::mx * 0.00045 * dt;
				updateViewMatrix();
			}
			
			if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_MIDDLE)) {
				if (InputManager::lastY < 0) {
					m_Position -= upVec * dt * cameraMouseMoveSpeed;
				} else if (InputManager::lastY > 0) {
					m_Position += upVec * dt * cameraMouseMoveSpeed;
				}
				if (InputManager::lastX > 0) {
					m_Position -= rightVec * dt * cameraMouseMoveSpeed;
				} else if (InputManager::lastX < 0) {
					m_Position += rightVec * dt * cameraMouseMoveSpeed;
				}
				updateViewMatrix();
			}
		}
	}

	void Camera::updateViewMatrix()
	{
		Matrix4 rotation = Matrix4::rotation(this->m_Rotation.x, this->m_Rotation.y, 0.0f);
		m_CamTarget = Vector4::normalize(Vector4(0.0f, 0.0f, 1.0f, 0.0f) * rotation) + m_Position;
		m_ViewMatrix = Matrix4::lookAt(m_Position, m_CamTarget, Vector4(0.0f, 1.0f, 0.0f, 0.0f));

		Matrix4 temp = Matrix4::rotation(this->m_Rotation.x, this->m_Rotation.y, this->m_Rotation.z);

		forwardVec = Vector4(0.0f, 0.0f, 1.0f, 0.0f) * temp;
		rightVec = Vector4(1.0f, 0.0f, 0.0f, 0.0f) * temp;
		upVec = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * temp;
	}
}
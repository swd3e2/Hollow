#include "Camera.h"

namespace Hollow {
	Camera::Camera()
	{
		m_Position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		m_Rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		updateViewMatrix();
	}

	void Camera::setProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
	{
		float fovRadians = fovDegrees / 360.0f * (Math::PI_F * 2);
		m_ProjectionMatrix = Matrix4::projection(fovRadians, aspectRatio, nearZ, farZ);
	}

	void Camera::setOrthoValues(float right, float left, float top, float bottom, float nearZ, float farZ)
	{
		m_ProjectionMatrix = Matrix4::orthographic(right, left, top, bottom, nearZ, farZ);
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
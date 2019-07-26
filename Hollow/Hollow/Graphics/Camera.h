#pragma once

#ifndef HW_CAMERA_H
#define HW_CAMERA_H

#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Vector4.h"
#include "Hollow/Math/Vector3.h"
#include "Hollow/Input/InputManager.h"
#include "Hollow/Math/EAngle.h"

namespace Hollow {
	class Camera
	{
	public:
		Camera(bool mainCamera);

		void setProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
		void setOrthoValues(float left, float right, float up, float down, float near, float far);

		const Matrix4& getViewMatrix() const { return m_ViewMatrix; }
		const Matrix4& getProjectionMatrix() const { return m_ProjectionMatrix; }

		const Vector4& getPosition() const { return m_Position; }
		const Vector4& getRotation() const { return m_Rotation; }

		Vector3 getPositionVec3() const { return Vector3(m_Position.x, m_Position.y, m_Position.z); }
		Vector3 getRotationVec3() const { return Vector3(m_Rotation.x, m_Rotation.y, m_Rotation.z); }

		void update(double dt);
	public:
		bool m_IsMainCamera;
		Vector4 m_Position;
		Vector4 m_Rotation;
		Vector4 m_CamTarget;
		Matrix4 m_ViewMatrix;
		Matrix4 m_ProjectionMatrix;

		Vector4 forwardVec;
		Vector4 rightVec;

		float bias = 0.000001f;
		float cameraMoveSpeed = 10.04f;
	public:
		void updateViewMatrix();
	};
}

#endif
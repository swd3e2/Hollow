#pragma once

#ifndef HW_CAMERA_H
#define HW_CAMERA_H

#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Vector3.h"
#include "Hollow/Math/Math.h"

namespace Hollow {
	class Camera
	{
	public:
		Camera();

		void setProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
		void setOrthoValues(float left, float right, float up, float down, float near, float far);

		const Matrix4& getViewMatrix() const { return m_ViewMatrix; }
		const Matrix4& getProjectionMatrix() const { return m_ProjectionMatrix; }

		inline const Vector4& getPosition() const { return m_Position; }

		inline void setPosition(const Vector3& position) 
		{ 
			m_Position = Hollow::Vector4(position.x, position.y, position.z, 1.0f);
			updateViewMatrix();
		}

		inline const Vector4& getRotation() const { return m_Rotation; }

		inline void setRotation(const Vector3& rotation) 
		{ 
			m_Rotation = rotation; 
			updateViewMatrix();
		}

		Vector3 getPositionVec3() const { return Vector3(m_Position.x, m_Position.y, m_Position.z); }
		Vector3 getRotationVec3() const { return Vector3(m_Rotation.x, m_Rotation.y, m_Rotation.z); }
		void updateViewMatrix();
	public:
		Vector4 m_Position;
		Vector4 m_Rotation;
		Vector4 m_CamTarget;
		Matrix4 m_ViewMatrix;
		Matrix4 m_ProjectionMatrix;

		Vector4 forwardVec;
		Vector4 rightVec;
		Vector4 upVec;
	};
}

#endif
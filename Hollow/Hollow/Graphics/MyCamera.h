#pragma once
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Vector4.h"
#include "Hollow/Math/Vector3.h"
#include "Hollow/Input/InputManager.h"
#include <DirectXMAth.h>
#include "Hollow/Math/EAngle.h"

class MyCamera
{
public:
	MyCamera(bool mainCamera);

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const Matrix4& GetViewMatrix() const;
	const Matrix4& GetProjectionMatrix() const;

	const Vector4& GetPosition() const { return position; }
	const Vector4& GetRotation() const { return rotation; }

	Vector3 GetPositionVec3() const
	{ 
		Vector3 temp;

		temp.x = position.x;
		temp.y = position.y;
		temp.z = position.z;

		return temp;
	}

	Vector3 GetRotationVec3() const
	{ 
		Vector3 temp;

		temp.x = position.x;
		temp.y = position.y;
		temp.z = position.z;
		
		return temp;
	}

	void Update(double dt);
public:
	bool mainCamera;
public:
	void UpdateViewMatrix();
	Vector4 position;
	Vector4 rotation;
	Vector4 camTarget;
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	
	EAngle angView;

	Vector4 forwardVec;
	Vector4 rightVec;
	Vector4 backVec;
	Vector4 leftVec;

	const float cameraMoveSpeed = 0.002f;
};
#ifndef __CAMERA__
#define __CAMERA__
#include <iostream>
#include "DirectXMath.h"
#include "Engine/Input/InputManager.h"

namespace Hollow {
	using namespace DirectX;

	class Camera
	{
	public:
		Camera();
		void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

		const XMMATRIX & GetViewMatrix() const;
		const XMMATRIX & GetProjectionMatrix() const;

		XMMATRIX GetViewMatrixCopy();
		XMMATRIX GetProjectionMatrixCopy();

		const XMVECTOR & GetPositionVector() const;
		const XMFLOAT3 & GetPositionFloat3() const;
		const XMVECTOR & GetRotationVector() const;
		const XMFLOAT3 & GetRotationFloat3() const;

		void SetPosition(const XMVECTOR & pos);
		void SetPosition(float x, float y, float z);
		void AdjustPosition(const XMVECTOR & pos);
		void AdjustPosition(float x, float y, float z);
		void SetRotation(const XMVECTOR & pos);
		void SetRotation(float x, float y, float z);
		void AdjustRotation(const XMVECTOR & pos);
		void AdjustRotation(float x, float y, float z);
		void SetLookAtPos(XMFLOAT3 lookAtPos);

		void Update();
		XMVECTOR & getLookAtVector();
		//bool RawMouseInput(const RawMouseEvent &arg);
	private:
		void UpdateViewMatrix();
		XMVECTOR posVector;
		XMVECTOR rotVector;
		XMFLOAT3 pos;
		XMFLOAT3 rot;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;

		const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		const float cameraSpeed = 0.05f;

		XMVECTOR camTarget;
		XMVECTOR vec_forward;
		XMVECTOR vec_left;
		XMVECTOR vec_right;
		XMVECTOR vec_backward;
	};
}

#endif
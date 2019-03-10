#pragma once
#include <DirectXMath.h>
#include "Hollow/Platform.h"

class HOLLOW_API Transform
{
public:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
public:
	Transform(DirectX::XMFLOAT3&& position, DirectX::XMFLOAT3&& scale, DirectX::XMFLOAT3&& rotation);
	Transform(float* position, float* scale, float* rotation);
	Transform();

	void setPosition(float px, float py, float pz);
	void setScale(float sx, float sy, float sz);
	void setRotation(float rx, float ry, float rz);
	void setTransform(float* position, float* scale, float* rotation);
};
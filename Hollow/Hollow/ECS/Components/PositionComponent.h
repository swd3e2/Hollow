#pragma once
#include "Hollow/ECS/Component.h"
#include "directXMath.h"

class PositionComponent : public Hollow::Component<PositionComponent>
{
public:
	PositionComponent(DirectX::XMFLOAT3&& position, DirectX::XMFLOAT3&& scale, DirectX::XMFLOAT3&& rotation) :
		position(position), scale(scale), rotation(rotation)
	{}

	PositionComponent(float* position, float* scale, float* rotation)
	{
		this->setTransform(position, scale, rotation);
	}

	void setTransform(float* position, float* scale, float* rotation)
	{
		this->position.x = position[0];
		this->position.y = position[1];
		this->position.z = position[2];

		this->scale.x = scale[0];
		this->scale.y = scale[1];
		this->scale.z = scale[2];

		this->rotation.x = rotation[0];
		this->rotation.y = rotation[1];
		this->rotation.z = rotation[2];
	}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
};

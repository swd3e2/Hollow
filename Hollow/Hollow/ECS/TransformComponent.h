#pragma once

#ifndef HW_TRANSFORM_COMPONENT_H
#define HW_TRANSFORM_COMPONENT_H

#include "Component.h"
#include <DirectXMath.h>

class TransformComponent : public Component<TransformComponent>
{
public:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
public:
	TransformComponent(DirectX::XMFLOAT3 && position, DirectX::XMFLOAT3 && scale, DirectX::XMFLOAT3 && rotation) :
		position(position), scale(scale), rotation(rotation)
	{}

	TransformComponent(float * position, float * scale, float * rotation)
	{
		this->setTransform(position, scale, rotation);
	}

	TransformComponent()
	{
		position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	}

	void setPosition(float px, float py, float pz)
	{
		this->position.x = px;
		this->position.y = py;
		this->position.z = pz;
	}

	void setScale(float sx, float sy, float sz)
	{
		this->scale.x = sx;
		this->scale.y = sy;
		this->scale.z = sz;
	}

	void setRotation(float rx, float ry, float rz)
	{
		this->rotation.x = rx;
		this->rotation.y = ry;
		this->rotation.z = rz;
	}

	void setTransform(float * position, float * scale, float * rotation)
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
};

#endif
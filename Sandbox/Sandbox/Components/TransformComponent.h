#pragma once

#ifndef HW_TRANSFORM_COMPONENT_H
#define HW_TRANSFORM_COMPONENT_H

#include <Hollow/ECS/Component.h>
#include <Hollow/Math/Vector3.h>
#include "Hollow/Math/Quaternion.h"

class TransformComponent : public Hollow::Component<TransformComponent>
{
public:
	Hollow::Vector3 position;
	Hollow::Vector3 scale;
	Hollow::Quaternion rotation;
public:
	TransformComponent() = default;

	TransformComponent(const Hollow::Vector3& position, const Hollow::Vector3& scale, const Hollow::Quaternion& rotation) :
		position(position), scale(scale), rotation(rotation)
	{}

	TransformComponent(float * position, float * scale, float * rotation)
	{
		this->setTransform(position, scale, rotation);
	}

	TransformComponent(const TransformComponent&) = default;
	TransformComponent(TransformComponent&&) = default;

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

	void setRotation(float rx, float ry, float rz, float rw)
	{
		this->rotation.x = rx;
		this->rotation.y = ry;
		this->rotation.z = rz;
		this->rotation.w = rw;
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
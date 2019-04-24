#pragma once

#ifndef HW_TRANSFORM_COMPONENT_H
#define HW_TRANSFORM_COMPONENT_H

#include "Component.h"
#include "Hollow/Math/Vector3.h"

class TransformComponent : public Component<TransformComponent>
{
public:
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;
public:
	TransformComponent(Vector3 && position, Vector3 && scale, Vector3 && rotation) :
		position(position), scale(scale), rotation(rotation)
	{}

	TransformComponent(float * position, float * scale, float * rotation)
	{
		this->setTransform(position, scale, rotation);
	}

	TransformComponent()
	{
		position.x = 0.0f;
		position.y = 0.0f;
		position.z = 0.0f;

		scale.x = 1.0f;
		scale.y = 1.0f;
		scale.z = 1.0f;

		rotation.x = 0.0f;
		rotation.y = 0.0f;
		rotation.z = 0.0f;
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
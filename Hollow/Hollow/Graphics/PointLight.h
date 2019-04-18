#pragma once
#include "Hollow/Platform.h"
#include "Hollow/ECS/Component.h"

struct PointLightStruct
{
	float position[3];
	float range;
	float color[3];
	float pad;
	float attenuation[3];
	float pad1;
};

class HOLLOW_API PointLight : public Component<PointLight>
{
public:
	PointLightStruct data;
	LightIcon lightIcon;
public:
	PointLight(ID3D11Device* device) : lightIcon (device) 
	{
		data.color[0] = 1.0f;
		data.color[1] = 1.0f;
		data.color[2] = 1.0f;

		data.range = 100.0f;

		data.position[0] = 0.0f;
		data.position[1] = 0.0f;
		data.position[2] = 0.0f;

		data.attenuation[0] = 0.0f;
		data.attenuation[1] = 0.0f;
		data.attenuation[2] = 0.0f;
	}

	void setPosition(float position[3])
	{
		data.position[0] = position[0];
		data.position[1] = position[1];
		data.position[2] = position[2];

		//lightIcon.renderable.transform->position = { position[0], position[1], position[2] };
	}

	void setAttenuation(float attenuation[3])
	{
		data.attenuation[0] = attenuation[0];
		data.attenuation[1] = attenuation[1];
		data.attenuation[2] = attenuation[2];
	}

	void setColor(float color[3])
	{
		data.color[0] = color[0];
		data.color[1] = color[1];
		data.color[2] = color[2];
	}
};
#pragma once

struct DirectionalLightStruct
{
	DirectionalLightStruct() { direction = {}; ambient = {}; }
	XMFLOAT3 direction;
	float pad;
	XMFLOAT4 ambient;
};

class DirectionalLight
{
public:
	DirectionalLightStruct data;
public:
	DirectionalLight()
	{

	}
};
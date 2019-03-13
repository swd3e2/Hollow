#pragma once
#include <string>

struct MaterialData
{
	// constant illumination color
	float diffuseColor[3] = {};
	float specularColor[3] = {};
	float ambientColor[3] = {};
	float specularExponent = 1000.0f;
};

class IMaterial
{
public:
	bool active = false;
	std::string name;
	std::string diffuse_texture;
	std::string normal_texture;
	MaterialData materialData;
};
#pragma once
#include <string>

struct MaterialData
{
	// constant illumination color
	float Ka[3] = {};
	float Ns;
	float Kd[3] = {};
	float pad;
	float Ks[3] = {};
	float pad2;
	float Ke[3] = {};
	float pad3;
	bool hasDiffuseTexture = false;
	bool hasNormalMap = false;
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
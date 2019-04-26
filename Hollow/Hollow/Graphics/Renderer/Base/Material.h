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
	int hasDiffuseTexture = 0;
	int hasNormalMap = 0;
	int hasSpecularMap = 0;
};

class Material
{
public:
	bool active = false;
	std::string name;
	std::string diffuse_texture;
	std::string normal_texture;
	std::string specular_texture;

	MaterialData materialData;
};
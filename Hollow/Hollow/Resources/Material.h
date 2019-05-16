#pragma once

#ifndef HW_MATERIAL_H
#define HW_MATERIAL_H

#include <string>
#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "Hollow/Graphics/ShaderManager.h"

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
	std::string diffuse_texture_name;
	std::string normal_texture_name;
	std::string specular_texture_name;

	MaterialData materialData;

	Texture* diffuse_texture;
	Texture* normal_texture;
	Texture* specular_texture;
	ShaderProgram* shader;

	Material()
	{
		shader = ShaderManager::instance()->getShader("default");
	}
};

#endif
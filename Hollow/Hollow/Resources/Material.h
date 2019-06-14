#pragma once

#ifndef HW_MATERIAL_H
#define HW_MATERIAL_H

#include <string>
#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "Hollow/Graphics/ShaderManager.h"

struct MaterialData
{
	// constant illumination color
	float Ka[3] = {0};
	float Ns = 0;
	float Kd[3] = {0};
	float pad = 0;
	float Ks[3] = {0};
	float pad2 = 0;
	float Ke[3] = {0};
	float pad3 = 0;
	int hasDiffuseTexture = 0;
	int hasNormalMap = 0;
	int hasSpecularMap = 0;
};

class Material
{
public:
	std::string name;

	MaterialData materialData;

	Texture* diffuseTexture;
	Texture* normalTexture;
	Texture* specularTexture;
	Texture* roughnesTexture;
	Texture* emisiveTexture;
	Texture* occlusionTexture;

	ShaderProgram* shader;

	Material()
	{
		shader = ShaderManager::instance()->getShader("default");
	}

	~Material()
	{}
};

#endif
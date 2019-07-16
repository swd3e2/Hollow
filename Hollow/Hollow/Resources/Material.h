#pragma once

#ifndef HW_MATERIAL_H
#define HW_MATERIAL_H

#include <string>
#include "Hollow/Graphics/Base/Texture.h"
#include "Hollow/Graphics/ShaderManager.h"
#include "Hollow/Math/Vector4.h"

namespace Hollow {
	struct MaterialData
	{
		Vector4 color;
		// constant illumination color
		float metallicFactor;
		float roughnessFactor;
		float emissiveFactor;
		float pad;
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

		Material()
		{
			//shader = ShaderManager::instance()->getShader("default");
		}

		~Material()
		{}
	};
}

#endif
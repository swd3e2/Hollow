#pragma once

#ifndef HW_MATERIAL_H
#define HW_MATERIAL_H

#include <string>
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Graphics/ShaderManager.h"
#include "Hollow/Math/Vector4.h"
#include "Hollow/Platform.h"

namespace Hollow {
	struct MaterialData
	{
		Vector4 color;
		// constant illumination color
		float metallicFactor;
		float roughnessFactor;
		float emissiveFactor;
		float ao;
		int hasDiffuseTexture = 0;
		int hasNormalTexture = 0;
		int hasSpecularMap = 0;

		MaterialData()
		{
			color = Vector4(1.0f, 1.0f, 1.0, 1.0f);
		}
	};

	class Material
	{
	public:
		std::string name;

		MaterialData materialData;

		s_ptr<Texture> diffuseTexture;
		s_ptr<Texture> normalTexture;
		s_ptr<Texture> specularTexture;
		s_ptr<Texture> roughnesTexture;
		s_ptr<Texture> emisiveTexture;
		s_ptr<Texture> occlusionTexture;
	public:
		~Material()
		{
			if (0) {}
		}
	};
}

#endif
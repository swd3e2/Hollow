#pragma once
#include "Texture.h"

namespace Hollow {

	// struct that get used in shaders for every kind of material
	struct MaterialData
	{
		bool has_texture;
		bool has_normal;
	};

	class Material
	{
	public:
		Material(std::string name, std::string diffuse_texture)
			: name(name)
		{}

		std::string name;
		Texture* diffuse_texture;
	};

}
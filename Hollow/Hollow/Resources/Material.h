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
		Material(std::string name = "default_material", std::string diffuse_texture = "default_texture_name.png")
			: name(name)
		{}
		Material(std::string name, Texture* diffuse_texture)
			: name(name), diffuse_texture(diffuse_texture)
		{}

		std::string name;
		Texture* diffuse_texture;
	};

}
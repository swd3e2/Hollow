#pragma once

#include <Hollow/Resources/Material.h>
#include <unordered_map>
#include <Hollow/Core/CModule.h>
#include <Hollow/Common/Log.h>
#include "vendor/tinygltf/json.hpp"
#include "Hollow/Common/FileSystem.h"

class MaterialManager : public Hollow::CModule<MaterialManager>
{
public:
	std::unordered_map<std::string, Hollow::s_ptr<Hollow::Material>> materials;
public:
	MaterialManager()
	{
		using json = nlohmann::json;
		auto j = json::parse(Hollow::FileSystem::getFileContent("E:/Hollow/Project1/materials.json"));

		for (int i = 0; i < j["materials"].size(); i++) {
			Hollow::s_ptr<Hollow::Material> material = create(j["materials"][i]["name"].get<std::string>());
			
			material->materialData.color = Hollow::Vector4(
				j["materials"][i]["base_color"][0].get<float>(),
				j["materials"][i]["base_color"][1].get<float>(),
				j["materials"][i]["base_color"][2].get<float>(),
				j["materials"][i]["base_color"][3].get<float>()
			);

			material->materialData.metallicFactor = j["materials"][i]["metallic"].get<float>();
			material->materialData.roughnessFactor = j["materials"][i]["roughness"].get<float>();
			//material.emissiveFactor = j["Materials"][i]["emissive"].get<float>();

			material->diffuseTextureFilename = j["materials"][i]["diffuse_texture"].get<std::string>();
			material->emissiveTextureFilename = j["materials"][i]["emissive_texture"].get<std::string>();
			material->normalTextureFilename = j["materials"][i]["normal_texture"].get<std::string>();
			material->occlusionTextureFilename = j["materials"][i]["occlusion_texture"].get<std::string>();
			material->roughnessTextureFilename = j["materials"][i]["roughness_texture"].get<std::string>();
			material->specularTextureFilename = j["materials"][i]["specular_texture"].get<std::string>();


			if (material->diffuseTextureFilename.size()) {
				material->diffuseTexture = TextureManager::instance()->create2DTextureFromFile("E:/Hollow/Sandbox/Sandbox/Resources/Textures/" + material->diffuseTextureFilename, 5);
				if (material->diffuseTexture != nullptr) {
					material->materialData.hasDiffuseTexture = true;
				}
			}
			if (material->normalTextureFilename.size()) {
				material->normalTexture = TextureManager::instance()->create2DTextureFromFile("E:/Hollow/Sandbox/Sandbox/Resources/Textures/" + material->normalTextureFilename, 5);
				if (material->normalTexture != nullptr) {
					material->materialData.hasNormalTexture = true;
				}
			}


			materials[material->name] = material;
		}
	}

	Hollow::s_ptr<Hollow::Material> create(const std::string materialName)
	{
		if (has(materialName)) {
			return get(materialName);
		}

		HW_INFO("New material {}", materialName.c_str());
		Hollow::s_ptr<Hollow::Material> material = std::make_shared<Hollow::Material>();
		material->name = materialName;
		materials[materialName] = material;

		return material;
	}

	bool has(const std::string materialName)
	{
		return materials.find(materialName) != materials.end();
	}
	
	Hollow::s_ptr<Hollow::Material> get(const std::string materialName)
	{
		return materials[materialName];
	}
};
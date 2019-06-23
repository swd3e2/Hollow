#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include <string>
#include <fstream>
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Importer/gltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Graphics/HardwareBufferManager.h"
#include "Hollow/Common/Helper.h"
#include "Hollow/Resources/Material.h"

using json = nlohmann::json;

struct RenderableObject {
	Hollow::VertexBuffer* vBuffer;
	Hollow::IndexBuffer* iBuffer;
	int material;
	int id;
};

class RenderableComponent : public Hollow::Component<RenderableComponent>
{
public:
	std::vector<RenderableObject> renderables;
	std::vector<Hollow::Material> materials;
	std::string filename;
public:
	RenderableComponent(std::string filename) :
		filename(filename)
	{
		using namespace Hollow;

		auto j = json::parse(FileSystem::getFileContent(filename));

		std::string binFilename = j["Data"]["filename"].get<std::string>();

		const char* fileFolder = Helper::trim_to_last_line_entry(filename.c_str(), '/');
		std::string folder;

		if (fileFolder) {
			folder = fileFolder;
		}

		std::ifstream file(folder + binFilename, std::ios::binary);

		for (int i = 0; i < j["Meshes"].size(); i++) {
			RenderableObject renderable;
			renderable.id = i;
			renderable.material = j["Meshes"][i]["material"].get<int>();

			file.seekg(j["Meshes"][i]["vertices_offset"].get<size_t>(), std::fstream::beg);
			Hollow::Vertex* vertices = new Hollow::Vertex[j["Meshes"][i]["vertices_size"].get<size_t>()];
			file.read((char*)vertices, sizeof(Hollow::Vertex) * j["Meshes"][i]["vertices_size"].get<size_t>());

			renderable.vBuffer = HardwareBufferManager::instance()->createVertexBuffer(vertices, j["Meshes"][i]["vertices_size"].get<size_t>());

			file.seekg(j["Meshes"][i]["indices_offset"].get<size_t>(), std::fstream::beg);
			unsigned int* indices = new unsigned int[j["Meshes"][i]["indices_size"].get<size_t>()];
			file.read((char*)indices, sizeof(unsigned int) * j["Meshes"][i]["indices_size"].get<int>());

			renderable.iBuffer = HardwareBufferManager::instance()->createIndexBuffer(indices, j["Meshes"][i]["indices_size"].get<int>());

			renderables.push_back(renderable);

			delete[] vertices;
			delete[] indices;
		}

		file.close();

		for (auto& it : j["Materials"]) {
			Hollow::Material material;
			material.name = it["name"].get<std::string>();
			material.materialData.color = Hollow::Vector4(
				it["base_color"][0].get<float>(), 
				it["base_color"][1].get<float>(), 
				it["base_color"][2].get<float>(), 
				it["base_color"][3].get<float>()
			);

			material.materialData.metallicFactor = it["metallness"].get<float>();
			material.materialData.roughnessFactor = it["roughness"].get<float>();
			material.materialData.emissiveFactor = it["emissive"].get<float>();

			std::string diffuseTexture = it["diffuse_texture"].get<std::string>();
			if (diffuseTexture.size()) {
				material.diffuseTexture = Hollow::TextureManager::instance()->CreateTextureFromFile(diffuseTexture);
			}
			materials.push_back(material);
		}
	}
};
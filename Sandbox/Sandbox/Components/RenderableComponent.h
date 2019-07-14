#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Graphics/Base/VertexBuffer.h"
#include "Hollow/Graphics/Base/IndexBuffer.h"
#include <string>
#include "Hollow/Graphics/Vertex.h"
#include "vendor/tinygltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Graphics/HardwareBufferManager.h"
#include "Hollow/Common/Helper.h"
#include "Hollow/Resources/Material.h"
#include "Hollow/Resources/MeshManager.h"

#include <unordered_map>

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
	std::unordered_map<unsigned int,Hollow::Material> materials;
	std::string filename;
public:
	RenderableComponent() {}

	RenderableComponent(const std::string& filename) :
		filename(filename)
	{
		load(filename);
	}

	void load(const std::string& filename)
	{
		this->filename = filename;
		using namespace Hollow;
		Import::Model* model = MeshManager::instance()->import(filename.c_str());

		for (int i = 0; i < model->meshes.size(); i++) {
			RenderableObject renderable;
			renderable.id = i;
			renderable.material = model->meshes[i]->material;

			renderable.vBuffer = HardwareBufferManager::instance()->createVertexBuffer(model->meshes[i]->vertices.data(), model->meshes[i]->vertices.size());
			renderable.iBuffer = HardwareBufferManager::instance()->createIndexBuffer(model->meshes[i]->indices.data(), model->meshes[i]->indices.size());

			renderables.push_back(renderable);
		}

		for (auto& it : model->materials) {
			Hollow::Material material;
			material.name = it.second.name;
			material.materialData.color = it.second.baseColorFactor;

			material.materialData.metallicFactor = it.second.metallicFactor;
			material.materialData.roughnessFactor = it.second.roughnessFactor;
			material.materialData.emissiveFactor = it.second.emissiveFactor;

			std::string diffuseTexture = it.second.diffuseTexture;
			if (diffuseTexture.size()) {
				material.diffuseTexture = Hollow::TextureManager::instance()->CreateTextureFromFile(diffuseTexture);
			}
			materials[it.first] = material;
		}

		delete model;
	}
};
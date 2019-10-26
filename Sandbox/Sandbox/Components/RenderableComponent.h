#pragma once

#include <Hollow/ECS/Component.h>
#include <string>
#include <Hollow/Graphics/Vertex.h>
#include <vendor/tinygltf/json.hpp>
#include <Hollow/Common/FileSystem.h>
#include <Hollow/Graphics/HardwareBufferManager.h>
#include <Hollow/Common/Helper.h>
#include <Hollow/Resources/Material.h>
#include <unordered_map>
#include "Hollow/Graphics/TextureManager.h"
#include "Hollow/Platform.h"

class RenderableObject {
public:
	Hollow::s_ptr<Hollow::VertexBuffer> vBuffer;
	Hollow::s_ptr<Hollow::IndexBuffer> iBuffer;
	int material;
	int id;
};

class RenderableComponent : public Hollow::Component<RenderableComponent>
{
public:
	std::vector<RenderableObject*> renderables;
	std::unordered_map<unsigned int, Hollow::Material*> materials;
	Hollow::Vector3 A, B; // A - left near down, B - right far up
	std::string filename;
public:
	RenderableComponent() = default;

	RenderableComponent(const Hollow::s_ptr<Hollow::Import::Model>& model)
	{
		load(model);
	}

	virtual ~RenderableComponent()
	{
		for (auto& it : renderables) {
			delete it;
		}
		for (auto& it : materials) {
			delete it.second;
		}
		materials.clear();
	}

	void load(const Hollow::s_ptr<Hollow::Import::Model>& model)
	{
		if (model == nullptr) { return; }
		for (int i = 0; i < model->meshes.size(); i++) {
			RenderableObject* renderable = new RenderableObject();
			renderable->id = i;
			renderable->material = model->meshes[i]->material;

			renderable->vBuffer = Hollow::VertexBuffer::create({ model->meshes[i]->vertices.data(), model->meshes[i]->vertices.size(), sizeof(Hollow::Vertex) });
			renderable->iBuffer = Hollow::IndexBuffer::create({ model->meshes[i]->indices.data(), model->meshes[i]->indices.size(), Hollow::INDEX_FORMAT::UINT });

			renderables.push_back(renderable);
		}

		for (auto& it : model->materials) {
			Hollow::Material* material = new Hollow::Material;
			material->name = it.second.name;
			material->materialData.color = it.second.baseColorFactor;

			material->materialData.metallicFactor = it.second.metallicFactor;
			material->materialData.roughnessFactor = it.second.roughnessFactor;
			material->materialData.emissiveFactor = it.second.emissiveFactor;

			std::string diffuseTexture = it.second.diffuseTexture;
			if (diffuseTexture.size()) {
				material->materialData.hasDiffuseTexture = true;
				std::string filepath = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/" + diffuseTexture;

				if (Hollow::FileSystem::exists(filepath)) {
					Hollow::s_ptr<Hollow::Import::Texture> diffuse = Hollow::FreeImgImporter::instance()->import(filepath.c_str());

					Hollow::TEXTURE_DESC desc;
					desc.width = diffuse->width;
					desc.height = diffuse->height;
					desc.format = diffuse->bpp == 32 ? Hollow::TextureFormat::TF_BGRA8 : Hollow::TextureFormat::TF_BGR8;
					desc.type = Hollow::TextureType::TT_TEXTURE2D;
					desc.numMips = 5;

					material->diffuseTexture = Hollow::TextureManager::instance()->create(desc);
					material->diffuseTexture->update(diffuse->data.get());
					material->diffuseTexture->generateMipMap();
				} else {
					HW_ERROR("Texture file not found, {}", filepath.c_str());
				}
			}
			materials[it.first] = material;
		}
	}
};
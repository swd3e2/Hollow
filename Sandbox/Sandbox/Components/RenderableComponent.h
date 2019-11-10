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

class RenderSystem;
class HierarchyTab;

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
	//friend class RenderSystem;
	//friend class HierarchyTab;

	struct Node
	{
		int renderableId = -1;
		Node* parent;
		std::string name;
		Hollow::Vector3 translation;
		Hollow::Vector3 scale;
		Hollow::Quaternion rotation;
		Hollow::Matrix4 localTransform;
		Hollow::Matrix4 worldTransform;
		std::vector<Node*> childs;
	};
public:
	std::vector<Hollow::s_ptr<Node>> nodes;
	Hollow::s_ptr<Node> rootNode;

	std::vector<Hollow::s_ptr<RenderableObject>> renderables;
	std::unordered_map<unsigned int, Hollow::s_ptr<Hollow::Material>> materials;
	Hollow::Vector3 A, B; // A - left near down, B - right far up
	std::string filename;
	bool skinned = false;
public:
	RenderableComponent() = default;

	RenderableComponent(const Hollow::s_ptr<Hollow::Import::Model>& model)
	{
		load(model);
	}

	void load(const Hollow::s_ptr<Hollow::Import::Model>& model)
	{
		if (model == nullptr) { 
			return; 
		}
		skinned = model->skinned;

		processNodes(model);

		for (int i = 0; i < model->meshes.size(); i++) {
			Hollow::s_ptr<RenderableObject> renderable = std::make_shared<RenderableObject>();
			renderable->id = i;
			renderable->material = model->meshes[i]->material;

			renderable->vBuffer = Hollow::VertexBuffer::create({ model->meshes[i]->vertices.data(), model->meshes[i]->vertices.size(), sizeof(Hollow::Vertex) });
			renderable->iBuffer = Hollow::IndexBuffer::create({ model->meshes[i]->indices.data(), model->meshes[i]->indices.size(), Hollow::IndexFormat::IFT_UINT });

			renderables.push_back(renderable);
		}

		for (auto& it : model->materials) {
			Hollow::s_ptr<Hollow::Material> material = std::make_shared<Hollow::Material>();
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
		if (0) {}
	}
private:
	void processNodes(const Hollow::s_ptr<Hollow::Import::Model>& model)
	{
		rootNode = std::make_shared<Node>();
		processHierarchy(rootNode, model->rootNode.get(), nullptr, Hollow::Matrix4::identity());
	}

	void processHierarchy(const Hollow::s_ptr<Node>& node, Hollow::Import::Node* iNode, Node* parent, const Hollow::Matrix4& parentTransform)
	{
		node->parent = parent;
		node->renderableId = iNode->meshId;
		node->name = iNode->name;
		node->rotation = iNode->rotation;
		node->scale = iNode->scale;
		node->translation = iNode->translation;
		node->worldTransform = Hollow::Matrix4::transpose(iNode->transform * parentTransform);

		nodes.push_back(node);

		for (auto& it : iNode->childs) {
			Hollow::s_ptr<Node> childNode = std::make_shared<Node>();
			node->childs.push_back(childNode.get());
			processHierarchy(childNode, it, node.get(), node->worldTransform);
		}
	}
};
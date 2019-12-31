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
#include "Sandbox/TextureManager.h"

class RenderSystem;
class HierarchyTab;

class RenderableComponent : public Hollow::Component<RenderableComponent>
{
public:
	struct Transform
	{
		Hollow::Vector3 translation;
		Hollow::Vector3 scale;
		Hollow::Quaternion rotation;
		Hollow::Matrix4 worldTransform;
		bool hasChanged = true;
	};

	struct Mesh 
	{
	public:
		int id;
		int material;
		Hollow::s_ptr<Hollow::IndexBuffer> iBuffer;
		Hollow::s_ptr<Hollow::VertexBuffer> vBuffer;
	};

	struct Node
	{
		int id;
		int parentId = -1;
		int jointId = -1;
		int mesh = -1;

		std::string name;
		Transform transform;
		std::vector<int> childs;
	};
public:
	std::vector<Hollow::s_ptr<Node>> nodes;
	int rootNode;

	std::vector<Hollow::s_ptr<Mesh>> renderables;
	std::vector<Hollow::s_ptr<Hollow::Material>> materials;
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

		rootNode = model->rootNode;
		filename = model->filename;
		skinned = model->skinned;

		processNodes(model);

		for (int i = 0; i < model->meshes.size(); i++) {
			Hollow::s_ptr<Mesh> renderable = std::make_shared<Mesh>();
			renderable->id = i;
			renderable->material = model->meshes[i]->material;

			renderable->vBuffer = Hollow::VertexBuffer::create({ model->meshes[i]->vertices.data(), model->meshes[i]->vertices.size(), sizeof(Hollow::Vertex) });
			renderable->iBuffer = Hollow::IndexBuffer::create({ model->meshes[i]->indices.data(), model->meshes[i]->indices.size(), Hollow::IndexFormat::IFT_UINT });

			renderables.push_back(renderable);
		}

		for (auto& it : model->materials) {
			Hollow::s_ptr<Hollow::Material> material = std::make_shared<Hollow::Material>();
			material->name = it.name;
			material->materialData.color = it.baseColorFactor;

			material->materialData.metallicFactor = it.metallicFactor;
			material->materialData.roughnessFactor = it.roughnessFactor;
			material->materialData.emissiveFactor = it.emissiveFactor;

			if (it.diffuseTexture.size()) {
				material->diffuseTexture = TextureManager::instance()->create2DTextureFromFile("E:/Hollow/Sandbox/Sandbox/Resources/Textures/" + it.diffuseTexture, 5);
				if (material->diffuseTexture != nullptr) {
					material->materialData.hasDiffuseTexture = true;
				}
			}
			if (it.normalTexture.size()) {
				material->normalTexture = TextureManager::instance()->create2DTextureFromFile("E:/Hollow/Sandbox/Sandbox/Resources/Textures/" + it.normalTexture, 5);
				if (material->normalTexture != nullptr) {
					material->materialData.hasNormalTexture = true;
				}
			}
			materials.push_back(material);
		}
	}
private:
	void processNodes(const Hollow::s_ptr<Hollow::Import::Model>& model)
	{
		for (int i = 0; i < model->nodes.size(); i++) {
			Hollow::s_ptr<Node> node = std::make_shared<Node>();
			node->id = model->nodes[i]->id;
			node->jointId = model->nodes[i]->jointId;
			node->mesh = model->nodes[i]->meshId;
			node->name = model->nodes[i]->name;
			
			node->childs = model->nodes[i]->childs;

			node->transform.rotation = model->nodes[i]->rotation;
			node->transform.scale = model->nodes[i]->scale;
			node->transform.translation = model->nodes[i]->translation;

			nodes.push_back(node);
		}
	}
};
#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Importer/gltf/GLTFImporter.h>
#include <Hollow/Graphics/HardwareBufferManager.h>
#include <Hollow/Resources/Material.h>

struct GLTFRenderableObject
{
	Hollow::IndexBuffer* iBuffer;
	Hollow::VertexBuffer* vBuffer;
	Hollow::Material* material;

	GLTFRenderableObject(Hollow::IndexBuffer* iBuffer, Hollow::VertexBuffer* vBuffer) :
		iBuffer(iBuffer), vBuffer(vBuffer)
	{}
	~GLTFRenderableObject() {}
};

class GLTFRenderable : public Hollow::Component<GLTFRenderable>
{
public:
	Hollow::GLTF::Node* rootNode;
	std::vector<GLTFRenderableObject*> renderables;
public:
	GLTFRenderable(Hollow::GLTF::GLTFModel* model)
	{
		rootNode = model->rootNode;
		for (auto& mesh : model->meshes) {
			GLTFRenderableObject* gltfModel = new GLTFRenderableObject(
				Hollow::HardwareBufferManager::instance()->createIndexBuffer(mesh->indices.data(), mesh->indices.size()),
				Hollow::HardwareBufferManager::instance()->createVertexBuffer(mesh->vertices.data(), mesh->vertices.size())
			);

			// Material data
			gltfModel->material = new Hollow::Material();
			gltfModel->material->name = model->materials[mesh->material].name;

			if (model->materials[mesh->material].diffuseTexture.size()) {
				gltfModel->material->diffuseTexture = Hollow::TextureManager::instance()->CreateTextureFromFile(model->materials[mesh->material].diffuseTexture);
			}
			/*if (model->materials[mesh->material].normalTexture.size()) {
				gltfModel->material->normalTexture = TextureManager::instance()->CreateTextureFromFile(model->materials[mesh->material].normalTexture);
			}
			if (model->materials[mesh->material].specularTexture.size()) {
				gltfModel->material->specularTexture = TextureManager::instance()->CreateTextureFromFile(model->materials[mesh->material].specularTexture);
			}
			if (model->materials[mesh->material].roughnesTexture.size()) {
				gltfModel->material->roughnesTexture = TextureManager::instance()->CreateTextureFromFile(model->materials[mesh->material].roughnesTexture);
			}
			if (model->materials[mesh->material].emisiveTexture.size()) {
				gltfModel->material->emisiveTexture = TextureManager::instance()->CreateTextureFromFile(model->materials[mesh->material].emisiveTexture);
			}
			if (model->materials[mesh->material].occlusionTexture.size()) {
				gltfModel->material->occlusionTexture = TextureManager::instance()->CreateTextureFromFile(model->materials[mesh->material].occlusionTexture);
			}*/

			renderables.push_back(gltfModel);
		}

		delete model;
	}
};
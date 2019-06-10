#pragma once

#include "Component.h"
#include "Hollow/Importer/gltf/GLTFImporter.h"
#include "Hollow/Graphics/HardwareBufferManager.h"

struct GLTFRenderableObject
{
	IndexBuffer* iBuffer;
	VertexBuffer* vBuffer;

	GLTFRenderableObject(IndexBuffer* iBuffer, VertexBuffer* vBuffer) :
		iBuffer(iBuffer), vBuffer(vBuffer)
	{}
	~GLTFRenderableObject() {}
};

class GLTFRenderable : public Component<GLTFRenderable>
{
public:
	Hollow::Node* rootNode;
	std::vector<GLTFRenderableObject*> renderables;
public:
	GLTFRenderable(Hollow::GLTFModel* model)
	{
		rootNode = model->rootNode;
		for (auto& mesh : model->meshes) {
			std::vector<Vertex>& vertices = mesh->vertices;
			std::vector<unsigned int>& indices = mesh->indices;

			renderables.push_back(new GLTFRenderableObject(
				HardwareBufferManager::instance()->createIndexBuffer(mesh->indices.data(), mesh->indices.size()),
				HardwareBufferManager::instance()->createVertexBuffer(mesh->vertices.data(), mesh->vertices.size())
			));
		}
	}
};
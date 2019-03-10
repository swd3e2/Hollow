#pragma once
#include <vector>
#include "ECS/IEntity.h"
#include "Graphics/DirectX/D3DRenderable.h"
#include "Graphics/TextureManager.h"
#include "Graphics/SimpleVertex.h"
#include "Resources/Mesh.h"
#include "Graphics/Transform.h"

class SceneManager
{
private:
	std::vector<IRenderable*> sceneObjects;
public:
	SceneManager() {}
	~SceneManager() {}

	void StartUp()
	{

	}

	void Shutdown()
	{
		for (auto& it : sceneObjects)
			delete it;
	}

	void CreateSceneObject(ID3D11Device * device, Mesh * mesh, Transform* transform)
	{
		if (mesh == nullptr)
			return;

		D3DRenderable* renderable = new D3DRenderable(transform, mesh->name);
		for (auto& it : mesh->objects) {
			RenderableObject* object = new RenderableObject(it->name);
			object->buffer = new D3DBuffer(device, it->data, sizeof(SimpleVertex), it->numVertices, D3D11_BIND_VERTEX_BUFFER);
			D3DMaterial* mat = new D3DMaterial();
			D3DTexture* tex = TextureManager::instance()->CreateTexture(it->material.diffuse_texture);
			mat->SetDiffuseTexture(tex);
			object->material = mat;
			renderable->renderableObjects.push_back(object);
		}
		delete mesh;

		sceneObjects.push_back(renderable);
	}

	std::vector<IRenderable*>* GetSceneObjects()
	{
		return &sceneObjects;
	}

};
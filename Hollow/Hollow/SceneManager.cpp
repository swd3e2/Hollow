#include "SceneManager.h"

void SceneManager::StartUp()
{
}

void SceneManager::Shutdown()
{
	for (auto& it : sceneObjects)
		delete it;
}

void SceneManager::CreateSceneObject(ID3D11Device * device, Mesh * mesh, Transform * transform)
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

std::vector<IRenderable*>* SceneManager::GetSceneObjects()
{
	return &sceneObjects;
}
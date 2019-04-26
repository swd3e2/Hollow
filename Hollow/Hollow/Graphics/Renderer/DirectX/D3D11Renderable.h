#pragma once

#ifndef HW_D3D_RENDERABLE_H
#define HW_D3D_RENDERABLE_H

#include "Hollow/Graphics/Renderer/Base/Renderable.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Resources/Mesh.h"
#include "D3D11Material.h"
#include "Hollow/Resources/TextureManager.h"

struct RenderableObject
{
	RenderableObject(std::string name = "") :
		name(name), buffer(nullptr), material(nullptr)
	{}

	~RenderableObject()
	{
		delete buffer;
		delete material;
	}

	std::string name;
	D3D11Buffer* buffer;
	D3D11Material* material;
};

class D3DRenderable : public IRenderable
{
public:
	Hollow::Containers::Vector<RenderableObject*> renderableObjects;
	std::string name;
public:
	D3DRenderable(Mesh* mesh) :
		name(mesh->name)
	{
		for (auto& it : mesh->objects) {
			RenderableObject* object = new RenderableObject(it->name);
			object->buffer = new D3D11Buffer(it->data, sizeof(Vertex), it->numVertices, D3D11_BIND_VERTEX_BUFFER);
			D3D11Material* mat = new D3D11Material();
			if (it->material.materialData.hasDiffuseTexture) {
				D3DTexture* tex = TextureManager::instance()->CreateTexture(it->material.diffuse_texture);
				if (tex != nullptr)
					mat->SetDiffuseTexture(tex);
			}
			if (it->material.materialData.hasNormalMap) {
				D3DTexture* tex = TextureManager::instance()->CreateTexture(it->material.normal_texture);
				if (tex != nullptr)
					mat->SetNormalTexture(tex);
			}
			if (it->material.materialData.hasSpecularMap) {
				D3DTexture* tex = TextureManager::instance()->CreateTexture(it->material.specular_texture);
				if (tex != nullptr)
					mat->SetSpecularTexture(tex);
			}
			object->material = mat;
			mat->materialData = it->material.materialData;

			this->renderableObjects.push_back(object);
		}
		delete mesh;
	}

	D3DRenderable()
	{}

	~D3DRenderable()
	{
		for (auto& it : renderableObjects)
			delete it;
	}
};

#endif
#pragma once
#include "Hollow/ECS/Component.h"
#include "directXMath.h"
#include "Hollow/Graphics/DirectX/D3DRenderable.h"
#include "Hollow/Resources/Mesh.h"
#include "Hollow/Graphics/TextureManager.h"
#include <vector>

class D3DRenderComponent : public Hollow::Component<D3DRenderComponent>
{
private:
	bool isValid = false;
public:
	D3DRenderComponent(ID3D11Device * device, Mesh * mesh)
	{
		if (mesh == nullptr)
			return;

		for (auto& it : mesh->objects) {
			RenderableObject* object = new RenderableObject();
			object->buffer = new D3DBuffer(device, it->data, sizeof(SimpleVertex), it->numVertices, D3D11_BIND_VERTEX_BUFFER);
			D3DMaterial* mat = new D3DMaterial();
			D3DTexture* tex = TextureManager::instance()->CreateTexture(it->material.diffuse_texture);
			mat->SetDiffuseTexture(tex);
			object->material = mat;
			renderable.renderableObjects.push_back(object);
		}
		delete mesh;
		isValid = true;
	}

	D3DRenderable renderable;
};

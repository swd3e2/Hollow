#pragma once

#ifndef HW_LIGHT_ICON_H
#define HW_LIGHT_ICON_H

#include "Hollow/Graphics/DirectX/D3DRenderable.h"
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Resources/TextureManager.h"
#include "Hollow/Resources/ShaderManager.h"

class LightIcon
{
public:
	D3DRenderable renderable;

	LightIcon(ID3D11Device* device)
	{
		Vertex vertices[] = {
				{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
				{  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
				{  1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
				{  1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
				{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
				{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
		};
		renderable.name = "Icon";

		RenderableObject* object = new RenderableObject();
		D3DMaterial* mat = new D3DMaterial();
		D3DTexture* tex = TextureManager::instance()->CreateTexture("light.png");
		mat->SetDiffuseTexture(tex);
		mat->pixelShader = ShaderManager::instance()->getPixelShader("iconPs");
		object->material = mat;
		object->name = "Icon";
		object->buffer = new D3DBuffer(device, vertices, sizeof(Vertex), 6, D3D11_BIND_VERTEX_BUFFER);
		renderable.renderableObjects.push_back(object);
		// renderable.transform->setScale(0.2f, 0.2f, 0.2f);
	}
};

#endif
#pragma once

#ifndef HW_RENDERABLE_COMPONENT_H
#define HW_RENDERABLE_COMPONENT_H

#include "Component.h"
#include "Hollow/Graphics/DirectX/D3DRenderable.h"

class RenderableComponent : public Component<RenderableComponent>
{
public:
	RenderableComponent(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Mesh* mesh)
		: renderable(device, deviceContext, mesh)
	{}

	D3DRenderable renderable;
};

#endif
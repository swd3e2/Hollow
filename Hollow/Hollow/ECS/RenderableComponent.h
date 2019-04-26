#pragma once

#ifndef HW_RENDERABLE_COMPONENT_H
#define HW_RENDERABLE_COMPONENT_H

#include "Component.h"
#include "Hollow/Graphics/DirectX/D3DRenderable.h"

class RenderableComponent : public Component<RenderableComponent>
{
public:
	RenderableComponent(Mesh* mesh)
		: renderable(mesh)
	{}

	D3DRenderable renderable;
};

#endif
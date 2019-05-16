#pragma once

#ifndef HW_RENDERABLE_COMPONENT_H
#define HW_RENDERABLE_COMPONENT_H

#include "Component.h"
#include "Hollow/Resources/Mesh/Mesh.h"

class RenderableComponent : public Component<RenderableComponent>
{
public:
	Mesh* mesh;
public:
	RenderableComponent()
	{}

	RenderableComponent(MeshData* data)
	{
		mesh = new Mesh(data);
	}

};

#endif
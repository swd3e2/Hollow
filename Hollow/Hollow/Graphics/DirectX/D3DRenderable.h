#pragma once
#include "Hollow/Graphics/IRenderable.h"
#include "D3DBuffer.h"
#include "Hollow/Containers/vector.h"
#include "D3DBuffer.h"
#include "D3DMaterial.h"

struct RenderableObject
{
	~RenderableObject()
	{
		delete buffer;
		delete material;
	}

	D3DBuffer* buffer;
	D3DMaterial* material;
};

class D3DRenderable : IRenderable
{
private:
	D3DMaterial* material;
public:
	Hollow::Containers::Vector<RenderableObject*> renderableObjects;
public:
	D3DRenderable()
	{
	}

	~D3DRenderable()
	{
		for (auto& it : renderableObjects)
			delete it;
	}
};
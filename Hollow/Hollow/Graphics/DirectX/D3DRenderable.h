#pragma once
#include "Hollow/Graphics/IRenderable.h"
#include "D3DBuffer.h"
#include "Hollow/Containers/vector.h"
#include "D3DBuffer.h"
#include "D3DMaterial.h"
#include "Hollow/Graphics/Transform.h"

struct RenderableObject
{
	RenderableObject(std::string name = "") :
		name(name)
	{}

	~RenderableObject()
	{
		delete buffer;
		delete material;
	}

	std::string name;
	D3DBuffer* buffer;
	D3DMaterial* material;
};

class D3DRenderable : public IRenderable
{
public:
	Hollow::Containers::Vector<RenderableObject*> renderableObjects;
	Transform* transform;
	std::string name;
public:
	D3DRenderable(Transform* transform, std::string name = "") : 
		transform(transform), name(name)
	{}

	D3DRenderable()
	{
		transform = new Transform();
	}

	~D3DRenderable()
	{
		for (auto& it : renderableObjects)
			delete it;
	}
};
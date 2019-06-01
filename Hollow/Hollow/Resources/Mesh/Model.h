#pragma once

#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include "Hollow/Resources/Material.h"

class Model
{
public:
	std::string name;
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
	Material* material;
public:
	Model() {}
	~Model()
	{
		delete vBuffer;
		delete iBuffer;
		delete material;
	}
};
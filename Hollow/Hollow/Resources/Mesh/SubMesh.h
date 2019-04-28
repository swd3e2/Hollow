#pragma once


#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/Material.h"

class SubMesh
{
public:
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
	Material* material;
};
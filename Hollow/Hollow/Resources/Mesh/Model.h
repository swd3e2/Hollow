#pragma once

#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include "Hollow/Resources/Material.h"

class HOLLOW_API Model
{
public:
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
	Material* material;
};
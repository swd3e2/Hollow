#pragma once

#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/Material.h"
#include "Bone.h"
#include <unordered_map>

class HOLLOW_API SubMesh
{
public:
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
	Material* material;
};
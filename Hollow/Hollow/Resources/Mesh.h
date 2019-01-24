#pragma once
#include "Hollow/Graphics/BufferTemplate/IndexBuffer.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"
#include "Hollow/Graphics/SimpleVertex.h"
#include "Hollow/Containers/vector.h"
#include "Material.h"

namespace Hollow {

	struct MeshModel {
		MeshModel(ID3D11Device* device, SimpleVertex* data, UINT numVertices)
			: buffer(device, data, numVertices)
		{}

		VertexBuffer<SimpleVertex> buffer;
		Material* material;
	};

	struct Mesh
	{
		Containers::Vector<MeshModel*> objects;
	};

}
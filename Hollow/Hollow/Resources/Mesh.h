#pragma once
#include "Hollow/Graphics/BufferTemplate/IndexBuffer.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"
#include "Hollow/Containers/vector.h"
#include "Material.h"
#include <string>

namespace Hollow {

	struct MeshModel {
		MeshModel(ID3D11Device* device, std::string name, SimpleVertex* data, UINT numVertices)
			: buffer(device, data, numVertices), name(name)
		{}

		std::string name;
		VertexBuffer<SimpleVertex> buffer;
		Material* material;
	};

	struct Mesh
	{
		~Mesh()
		{
			if (objects.size() > 0)
			{
				for (auto& it : objects)
					delete it;
			}
		}

		Containers::Vector<MeshModel*> objects;
	};

}
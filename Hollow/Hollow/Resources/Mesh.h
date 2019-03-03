#pragma once
#include "Hollow/Graphics/SimpleVertex.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Graphics/IMaterial.h"

struct MeshModel {
	MeshModel(SimpleVertex* data, size_t numVertices) :
		data(data), numVertices(numVertices)
	{}

	SimpleVertex* data;
	size_t numVertices;
	IMaterial material;
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

	Hollow::Containers::Vector<MeshModel*> objects;
};
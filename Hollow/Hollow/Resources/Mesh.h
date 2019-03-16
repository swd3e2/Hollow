#pragma once
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Graphics/IMaterial.h"
#include "Hollow/Platform.h"

struct MeshModel {
	MeshModel(Vertex* data, UINT numVertices, std::string name = "") :
		data(data), numVertices(numVertices), name(name)
	{}

	std::string name;
	Vertex* data;
	UINT numVertices;
	IMaterial material;
};

struct Mesh
{
	Mesh(std::string name = "") :
		name(name)
	{}

	~Mesh()
	{
		if (objects.size() > 0)
		{
			for (auto& it : objects)
			{
				delete it;
			}
		}
	}

	Hollow::Containers::Vector<MeshModel*> objects;
	std::string name;
};
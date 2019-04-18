#pragma once

#ifndef HW_MESH_H
#define HW_MESH_H

#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Graphics/IMaterial.h"
#include "Hollow/Platform.h"

struct MeshModel {
	MeshModel(Vertex* data, UINT numVertices, std::string name = "") :
		data(data), numVertices(numVertices), name(name)
	{}
	MeshModel(size_t* indices, UINT numVertices, std::string name = "") :
		indices(indices), numVertices(numVertices), name(name)
	{}

	std::string name;
	Vertex* data;
	size_t* indices;
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

	Vertex* data;
	Hollow::Containers::Vector<MeshModel*> objects;
	std::string name;
};

#endif
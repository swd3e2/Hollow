#pragma once

#ifndef HW_MESH_H
#define HW_MESH_H

#include "Hollow/Containers/vector.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Resources/Material.h"
#include "Hollow/Graphics/HardwareBufferManager.h"
#include "Hollow/Graphics/TextureManager.h"
#include "Hollow/Graphics/Vertex.h"

namespace Hollow {
	class Model
	{
	public:
		std::string name;
		s_ptr<VertexBuffer> vBuffer;
		s_ptr<IndexBuffer> iBuffer;
		Material* material;
	public:
		Model() {}
		~Model()
		{
			delete material;
		}
	};

	class Mesh
	{
	public:
		std::vector<Model*> models;
		std::string filename;
		Vertex A, B; // A - left near down, B - right far up
	public:
		Mesh() {}

		~Mesh()
		{
			for (auto& it : models) {
				delete it;
			}
		}
	};
}

#endif
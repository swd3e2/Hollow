#pragma once

#ifndef HW_MESH_H
#define HW_MESH_H

#include "Hollow/Containers/vector.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Resources/Material.h"
#include "Hollow/Graphics/HardwareBufferManager.h"
#include "Hollow/Graphics/TextureManager.h"

namespace Hollow {
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

	class Mesh
	{
	public:
		std::vector<Model*> models;
		std::string filename;
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
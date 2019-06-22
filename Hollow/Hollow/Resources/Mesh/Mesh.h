#pragma once

#ifndef HW_MESH_H
#define HW_MESH_H

#include "Model.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Resources/Material.h"
#include "Hollow/Graphics/HardwareBufferManager.h"
#include "Hollow/Graphics/TextureManager.h"

namespace Hollow {
	class Mesh
	{
	public:
		int numModels;
		std::vector<Model*> models;
		std::string filename;
	public:
		Mesh() : numModels(0) {}

		~Mesh()
		{
			for (auto& it : models) {
				delete it;
			}
		}
	};
}

#endif
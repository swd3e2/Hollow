#pragma once

#ifndef HW_MESH_MANAGER_H
#define HW_MESH_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Hollow/Importer/AssimpImporter.h"
#include "Hollow/Importer/GLTFImporter.h"
#include "Hollow/Importer/HollowModelImporter.h"

namespace Hollow {
	class MeshManager : public CModule<MeshManager>
	{
	private:
		AssimpImporter assimpImporter;
		GLTFImporter gltfImporter;
		HollowModelImporter hwImporter;
	public:
		Import::Model* import(const char* filename)
		{
			Import::Model* model = nullptr;

			if (strcmp(filename + strlen(filename) - 5, ".gltf") == 0) {
				model = gltfImporter.import(filename);
			} else if (strcmp(filename + strlen(filename) - 5, ".json") == 0) {
				model = hwImporter.import(filename);
			} else {
				model = assimpImporter.import(filename);
			}

			return model;
		}
	};
}

#endif
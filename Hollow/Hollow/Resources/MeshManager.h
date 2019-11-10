#pragma once

#ifndef HW_MESH_MANAGER_H
#define HW_MESH_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Hollow/Import/GLTFImporter.h"
#include "Hollow/Import/HollowModelImporter.h"
#include "Hollow/Platform.h"

namespace Hollow {
	class MeshManager : public CModule<MeshManager>
	{
	private:
		GLTFImporter gltfImporter;
		HollowModelImporter hwImporter;
	public:
		s_ptr<Import::Model> import(const char* filename)
		{
			s_ptr<Import::Model> model;

			if (!FileSystem::exists(filename)) {
				return nullptr;
			}

			if (strcmp(filename + strlen(filename) - 5, ".gltf") == 0) {
				model = gltfImporter.import(filename);
			} else if (strcmp(filename + strlen(filename) - 5, ".json") == 0) {
				model = hwImporter.import(filename);
			}

			return model;
		}
	};
}

#endif
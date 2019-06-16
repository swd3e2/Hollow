#pragma once 

#include "Hollow/Resources/Mesh/Mesh.h"
#include "Hollow/Graphics/Vertex.h"
#include <string>
#include <unordered_map>
#include "Hollow/Math/Matrix4.h"

namespace Hollow {
	struct LoadedMaterial
	{
		std::string name;
		std::string diffuseTextureName;
		std::string normalTextureName;
		std::string specularTextureName;

		bool hasDiffueTexture = false;
		bool hasNormalTexture = false;
		bool hasSpecularTexture = false;
	};

	struct MeshData
	{
		std::vector<Vertex>* vertices;
		std::vector<unsigned int>* indices;
		std::vector<LoadedMaterial*> materials;
		std::vector<std::string> modelNames;
		int numModels;
	};

	class MeshImportData
	{
	public:
		MeshData* meshData;
		std::string filename;
	public:
		MeshImportData() :
			meshData(nullptr)
		{}
		inline bool hasMesh() { return meshData != nullptr; }
	};
}
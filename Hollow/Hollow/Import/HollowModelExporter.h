#pragma once
#include "Mesh.h"
#include <vector>
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Platform.h"
#include <string>
#include "Hollow/Math/Vector3.h"

namespace Hollow 
{
	namespace Export
	{
		struct Node
		{
			int id;
			int mesh;
		};

		struct Joint
		{
			int id;
		};

		struct Material
		{
			std::string name;

			Vector3 color;
			Vector3 emissiveColor;

			float metallic;
			float specular;
			float roughness;
			float ambientOcclusion;

			std::string diffuseTexture;
			std::string normalTexture;
			std::string specularTexture;
			std::string roughnesTexture;
			std::string emisiveTexture;
			std::string occlusionTexture;
		};

		struct Mesh
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			int material;
		};

		struct Model
		{
			std::vector<s_ptr<Mesh>> meshes;
			std::vector<Material> materials;
		};
	}
	

	class HollowModelExporter
	{
	public:
		void export2()
		{

		}
	};
}
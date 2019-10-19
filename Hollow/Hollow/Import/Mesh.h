#pragma once

#include <string>
#include <unordered_map>
#include "Hollow/Math/Vector4.h"
#include "Hollow/Graphics/Vertex.h"
#include <limits>
#include <vector>
#include "Animation.h"

namespace Hollow {
	namespace Import {
		struct Material
		{
			std::string diffuseTexture;
			std::string normalTexture;
			std::string specularTexture;
			std::string roughnesTexture;
			std::string emisiveTexture;
			std::string occlusionTexture;

			std::string name;
			Vector4 baseColorFactor;
			float metallicFactor = 0.0f;
			float roughnessFactor = 0.0f;
			float emissiveFactor = 0.0f;

			unsigned int id;
		};

		struct Mesh
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::string name;
			unsigned int material;
			unsigned int id;
		};

		struct Model
		{
			std::vector<Mesh*> meshes;
			Vector3 A, B; // A - left near down, B - right far up
			std::unordered_map<unsigned int, Material> materials;
			std::vector<Animation*> animations;
			AnimationNode* rootNode;

			Model()
			{
				A = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
				B = Vector3(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
			}

			~Model()
			{
				for (auto& it : meshes) {
					delete it;
				}
				for (auto& it : animations) {
					delete it;
				}
				delete rootNode;
			}
		};
	}
}

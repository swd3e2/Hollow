#pragma once

#include "Hollow/Platform.h"
#include "Hollow/Math/Vector4.h"
#include "Hollow/Graphics/Vertex.h"
#include "Animation.h"

#include <unordered_map>
#include <limits>
#include <vector>
#include <string>

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
			Vector4 baseColorFactor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
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
			std::vector<s_ptr<Mesh>> meshes;
			std::unordered_map<int, Material> materials;
			std::unordered_map<int, s_ptr<AnimationNode>> nodes;
			std::vector<s_ptr<Animation>> animations;
			s_ptr<AnimationNode> rootNode;
			// A - left near down, B - right far up
			Vector3 A = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			Vector3 B = Vector3(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
		};
	}
}

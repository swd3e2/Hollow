#pragma once

#include "Hollow/Platform.h"
#include "Hollow/Math/Vector3.h"
#include "Hollow/Math/Vector4.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Graphics/Vertex.h"

#include <unordered_map>
#include <map>
#include <limits>
#include <vector>
#include <string>

namespace Hollow {
	namespace Import {
		struct Node 
		{
			int id;
			int jointId;
			int meshId = -1;
			std::string name;

			Vector3 translation;
			Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
			Quaternion rotation;

			std::vector<int> childs;
		};

		struct AnimationData
		{
			std::map<double, Quaternion> rotations;
			std::map<double, Vector3> positions;
			std::map<double, Vector3> scale;
		};

		struct Animation
		{
			std::map<int, s_ptr<AnimationData>> data;
			double duration = 0.0;
			std::string name;
		};

		struct Material
		{
			int id;

			std::string name;
			Vector4 baseColorFactor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
			float metallicFactor = 0.0f;
			float roughnessFactor = 0.0f;
			float emissiveFactor = 0.0f;

			std::string diffuseTexture;
			std::string normalTexture;
			std::string specularTexture;
			std::string roughnesTexture;
			std::string emisiveTexture;
			std::string occlusionTexture;
		};

		struct Mesh
		{
			int id;
			int material;
			bool hasTangent = false;
			std::string name;
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
		};

		struct Model
		{
			std::string filename;
			std::vector<s_ptr<Mesh>> meshes;
			std::vector<Material> materials;
			// Animation data
			std::vector<int> joints;
			std::vector<s_ptr<Animation>> animations;
			std::vector<Matrix4> inverseBindMatrices;
			int rootJoint;
			bool skinned = false;
			// Node hierarchy data
			int rootNode;
			std::vector<s_ptr<Node>> nodes;
			// A - left near down, B - right far up
			Vector3 A = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			Vector3 B = Vector3(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
		};
	}
}

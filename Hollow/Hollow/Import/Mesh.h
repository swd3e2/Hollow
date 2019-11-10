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
		struct BaseNode 
		{
			int id;
			int jointId;
			std::string name;
			Vector3 translation;
			Vector3 scale;
			Quaternion rotation;
			Matrix4 transform;
		};

		struct Joint : public BaseNode
		{
			Matrix4 localTransform;
			Matrix4 inverseBindMatrix;
			std::vector<Joint*> childs;
		};

		struct Node : public BaseNode
		{
			int meshId = -1;
			bool skinned;
			bool hasMatrix = false;
			std::vector<Node*> childs;
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

			int id;
		};

		struct Mesh
		{
			int id;
			int material;
			std::string name;
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
		};

		struct Model
		{
			std::vector<s_ptr<Mesh>> meshes;
			std::unordered_map<int, Material> materials;
			// Animation data
			std::unordered_map<int, s_ptr<Joint>> joints;
			std::vector<s_ptr<Animation>> animations;
			s_ptr<Joint> rootJoint;
			// Node hierarchy data
			std::unordered_map<int, s_ptr<Node>> nodes;
			s_ptr<Node> rootNode;
			bool skinned = false;
			// A - left near down, B - right far up
			Vector3 A = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			Vector3 B = Vector3(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
		};
	}
}

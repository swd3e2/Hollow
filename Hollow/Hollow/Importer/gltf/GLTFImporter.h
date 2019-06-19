#pragma once

#include "tiny_gltf.h"

#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Quaternion.h"
#include "Hollow/Graphics/Vertex.h"
#include "Animation.h"

#include <fstream>
#include <unordered_map>
#include <string>

#define COMPONENT_TYPE_UNSIGNED_SHORT 5123
#define COMPONENT_TYPE_UNSIGNED_INT 5125
#define COMPONENT_TYPE_FLOAT 5126

#define TRANSLATION 0
#define ROTATION 1
#define SCALE 2

namespace Hollow {
	namespace GLTF
	{
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
			float emmisiveFactor = 0.0f;

			unsigned int id;
		};

		struct LoadedMesh
		{
			std::vector<Vector3> normals;
			std::vector<Vector3> positions;
			std::vector<Vector2> texCoords;
			std::vector<float*>  weigths;
			std::vector<unsigned short*> joints;
			std::vector<unsigned int> indices;
			std::string name;
			unsigned int material;
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

		struct Node
		{
			std::vector<Node*> childrens;
			Matrix4 transformation;
			std::string name;
			// index to mesh
			int mesh = -1;
			Node(std::string& name) :
				name(name)
			{}
		};

		struct GLTFModel
		{
			std::vector<Mesh*> meshes;
			std::unordered_map<unsigned int, Material> materials;
			std::vector<Animation> animations;
			Node* rootNode;
			AnimationNode* rootAnimationNode;
		};

		struct Model
		{
			unsigned int meshIdCounter = 0;
			unsigned int nodeCounter = 0;
			unsigned int meshCounter = 0;
			std::vector<LoadedMesh*> meshes;
			std::unordered_map<unsigned int, Material> materials;
			std::vector<Animation> animations;
			Node* rootNode;
			AnimationNode* animationRootNode;
			std::vector<AnimationNode*> animationNodes;

			~Model()
			{
				for (auto& it : meshes) {
					delete it;
				}
			}
		};
	}

	class GLTFImporter
	{
	public:
		Hollow::GLTF::GLTFModel* import(const char* filename);

		bool hasMesh(tinygltf::Node node, tinygltf::Model& model);
		void processMesh(Hollow::GLTF::Node* node, tinygltf::Node childModelNode, Hollow::GLTF::Model& model, tinygltf::Model& tModel, std::ifstream& file);
		void load(Hollow::GLTF::Node* node, const tinygltf::Node& modelNode, tinygltf::Model& tModel, Hollow::GLTF::Model& model, std::ifstream& file);
		void processAnimation(Hollow::GLTF::Model& lModel, tinygltf::Model& model, std::ifstream& file);
		void processAnimationNode(Hollow::AnimationNode* node, const tinygltf::Node& modelNode, Hollow::GLTF::Model& lModel, tinygltf::Model& model);
		void processSkin(Hollow::GLTF::Model& lModel, tinygltf::Model& model, std::ifstream& file);
	};
}
#pragma once


#include "Animation.h"
#include "Mesh.h"
#include "Hollow/Common/Helper.h"
#include "vendor/tinygltf/tiny_gltf.h"
#include "Hollow/Platform.h"

#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#define COMPONENT_TYPE_UNSIGNED_SHORT 5123
#define COMPONENT_TYPE_UNSIGNED_INT 5125
#define COMPONENT_TYPE_FLOAT 5126

#define TRANSLATION 0
#define ROTATION 1
#define SCALE 2

namespace Hollow {
	namespace GLTF
	{
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

		struct LoadedModel
		{
			unsigned int meshIdCounter = 0;
			unsigned int nodeCounter = 0;
			unsigned int meshCounter = 0;
			std::vector<u_ptr<LoadedMesh>> meshes;
			std::unordered_map<unsigned int, Import::Material> materials;
			std::vector<Animation> animations;
			Node* rootNode;
			AnimationNode* animationRootNode;
			std::vector<AnimationNode*> animationNodes;
		};
	}

	class GLTFImporter
	{
	public:
		s_ptr<Import::Model> import(const char* filename);
		void prepareModel(GLTF::Node* node, const Matrix4& parentTransform, const s_ptr<Import::Model>& model);
		bool hasMesh(tinygltf::Node node, tinygltf::Model& model);
		void processMesh(GLTF::Node* node, tinygltf::Node childModelNode, GLTF::LoadedModel& model, tinygltf::Model& tModel, std::ifstream& file);
		void load(GLTF::Node* node, const tinygltf::Node& modelNode, tinygltf::Model& tModel, GLTF::LoadedModel& model, std::ifstream& file);
		void processAnimation(GLTF::LoadedModel& lModel, tinygltf::Model& model, std::ifstream& file);
		void processAnimationNode(AnimationNode* node, const tinygltf::Node& modelNode, GLTF::LoadedModel& lModel, tinygltf::Model& model);
		void processSkin(GLTF::LoadedModel& lModel, tinygltf::Model& model, std::ifstream& file);
	};
}
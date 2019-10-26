#pragma once


#include "Animation.h"
#include "Mesh.h"
#include "Hollow/Common/Helper.h"
#include "vendor/tinygltf/tiny_gltf.h"
#include "Hollow/Platform.h"
#include "Hollow/Common/FileSystem.h"

#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#define COMPONENT_TYPE_UNSIGNED_SHORT 5123
#define COMPONENT_TYPE_UNSIGNED_INT 5125
#define COMPONENT_TYPE_FLOAT 5126

namespace Hollow {
	class GLTFImporter
	{
	private:
		struct Node
		{
			int id;
			bool skinned;
			std::vector<Node*> childrens;
			Vector3 translation;
			Vector3 scale;
			Quaternion rotation;
			Matrix4 transformation;
			std::string name;
			int mesh = -1;
		};
	private:
		int meshCounter = 0;
		int nodeCounter = 0;
	public:
		s_ptr<Import::Model> import(const char* filename);
		void prepareModel(Node* node, const Matrix4& parentTransform, const s_ptr<Import::Model>& model);
		void processHierarchy(Node* node, const tinygltf::Node& modelNode, const tinygltf::Model& gltfModel, std::unordered_map<int, Node*>& nodes);
		void processAnimation(Import::Model* lModel, tinygltf::Model& model, std::ifstream& file);
		void processAnimationNode(Import::AnimationNode* node, const tinygltf::Node& modelNode, std::unordered_map<int, Import::AnimationNode*>& nodes, const tinygltf::Model& gltfModel);
		void processSkin(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary);
		void fixAnimation(Import::Model* model, std::unordered_map<int, Node*>& nodes);
		void processMeshes(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary);
		int getSkinByMesh(int mesh, const tinygltf::Node& node, const tinygltf::Model& model);
		std::string getBinaryFileFolder(std::string gltfFilePath);
	};
}
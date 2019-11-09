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
#include <algorithm>

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
			int jointId;
			int mesh = -1;
			bool skinned;
			Vector3 translation;
			Vector3 scale;
			Quaternion rotation;
			Matrix4 transformation;
			std::string name;
			std::vector<s_ptr<Node>> childrens;
		};
	private:
		int animationNodeCounter = 0;
		int meshCounter = 0;
		int nodeCounter = 0;
	public:
		s_ptr<Import::Model> import(const char* filename);
		void fixModel(const s_ptr<Node>& node, const Matrix4& parentTransform, Import::Model* model);
		void processHierarchy(s_ptr<Node>& node, const tinygltf::Node& modelNode, const tinygltf::Model& gltfModel, std::unordered_map<int, s_ptr<Node>>& nodes, std::vector<int>& meshesToLoad);
		void processAnimations(Import::Model* lModel, tinygltf::Model& model, std::ifstream& file);
		void processAnimationNode(s_ptr<Import::AnimationNode> node, const tinygltf::Node& modelNode, std::unordered_map<int, s_ptr<Import::AnimationNode>>& nodes, const tinygltf::Model& gltfModel);
		void processSkin(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary);
		void processMeshes(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary, std::vector<int>& meshesToLoad);
		int getSkinByMesh(int mesh, const tinygltf::Node& node, const tinygltf::Model& model);
		std::string getBinaryFileFolder(std::string gltfFilePath);
		int getJointByNode(int nodeId, const tinygltf::Model& gltfModel);
		int getNodeByJoint(int jointId, const tinygltf::Model& gltfModel);
		Matrix4 getTransformMatrix(const tinygltf::Node& modelNode);
	};
}
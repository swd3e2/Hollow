#pragma once

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
		int animationNodeCounter = 0;
		int meshCounter = 0;
		int nodeCounter = 0;
	public:
		s_ptr<Import::Model> import(const char* filename);
		void processHierarchy(s_ptr<Import::Node>& node, const tinygltf::Node& modelNode, Import::Model* model, const tinygltf::Model& gltfModel, std::vector<int>& meshesToLoad);
		void processAnimations(Import::Model* lModel, tinygltf::Model& model, std::ifstream& file);
		void processAnimationNode(s_ptr<Import::Joint>& node, const tinygltf::Node& modelNode, std::unordered_map<int, s_ptr<Import::Joint>>& joints, const tinygltf::Model& gltfModel);
		void processSkin(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary);
		void processMeshes(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary, std::vector<int>& meshesToLoad);
		int getSkinByMesh(int mesh, const tinygltf::Node& node, const tinygltf::Model& model);
		std::string getBinaryFileFolder(std::string gltfFilePath);
		int getJointByNode(int nodeId, const tinygltf::Model& gltfModel);
		int getNodeByJoint(int jointId, const tinygltf::Model& gltfModel);
		bool hasMesh(const tinygltf::Node& node, const tinygltf::Model& model);
		void getTransformMatrix(const tinygltf::Node& modelNode, const s_ptr<Import::BaseNode>& node);
	};
}
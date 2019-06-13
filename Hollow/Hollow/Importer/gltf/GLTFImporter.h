#pragma once

#include "tiny_gltf.h"

#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Quaternion.h"
#include "Hollow/Graphics/Vertex.h"

#include <fstream>
#include <unordered_map>
#include <string>

#define UNSIGNED_SHORT 5123
#define UNSIGNED_INT 5125
#define FLOAT 5126

#define TRANSLATION 0
#define ROTATION 1
#define SCALE 2

namespace Hollow {
	struct NodeKeyFrameData
	{
		Vector3 vec3Val;
		Quaternion quatVal;
	};

	struct NodeAnimationData
	{
		int nodeId;
		std::map<float, NodeKeyFrameData> data;
		
	};

	struct AnimationNode
	{
		int gltfId;
		int id;
		Matrix4 localTransform;
		std::vector<AnimationNode*> childrens;
	};

	struct Animation
	{
		std::unordered_map<int, NodeAnimationData> rotations;
		std::unordered_map<int, NodeAnimationData> translation;
		std::unordered_map<int, NodeAnimationData> scale;

		inline NodeKeyFrameData* findKeyFrame(double time, int bone, int type)
		{
			NodeAnimationData* animationData = nullptr;
			NodeKeyFrameData* frame = nullptr;

			if (type == TRANSLATION) {
				animationData = &translation[bone];
			} else if (type == ROTATION) {
				animationData = &rotations[bone];
			} else if (type == SCALE) {
				animationData = &scale[bone];
			}

			if (animationData == nullptr) { return nullptr; }

			for (auto& it : animationData->data) {
				if (it.first <= time) {
					frame = &it.second;
				}
			}

			if (animationData == nullptr && animationData->data.size()) {
				animationData->data[animationData->data.size() - 1];
			}

			return frame;
		}

		inline NodeKeyFrameData* findKeyNextFrame(double time, int bone, int type)
		{
			NodeAnimationData* animationData = nullptr;
			NodeKeyFrameData* frame = nullptr;

			if (type == TRANSLATION) {
				animationData = &translation[bone];
			}
			else if (type == ROTATION) {
				animationData = &rotations[bone];
			}
			else if (type == SCALE) {
				animationData = &scale[bone];
			}

			if (animationData == nullptr) { return nullptr; }

			for (auto& it : animationData->data) {
				if (it.first > time) {
					frame = &it.second;
					break;
				}
			}

			if (animationData == nullptr && animationData->data.size()) {
				animationData->data[animationData->data.size() - 1];
			}

			return frame;
		}
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
		unsigned int material;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int material;
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
		AnimationNode* animationRootNode;

		~GLTFModel()
		{
			for (auto& it : meshes) {
				delete it;
			}
		}
	};

	struct Model
	{
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
	Hollow::GLTFModel* import(const char* filename);

	bool hasMesh(tinygltf::Node node, tinygltf::Model& model);
	void processMesh(Hollow::Node* node, tinygltf::Node childModelNode, Hollow::Model& model, tinygltf::Model& tModel, std::ifstream& file);
	void load(Hollow::Node* node, const tinygltf::Node& modelNode, tinygltf::Model& tModel, Hollow::Model& model, std::ifstream& file);
	void processAnimation(Hollow::Model& lModel, tinygltf::Model& model, std::ifstream& file);
	void processAnimationNode(Hollow::AnimationNode* node, const tinygltf::Node& modelNode, Hollow::Model& lModel, tinygltf::Model& model);
	void processSkin(Hollow::Model& lModel, tinygltf::Model& model, std::ifstream& file);
};
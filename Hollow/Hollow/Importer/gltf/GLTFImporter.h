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
		double time;
	};

	struct KeyFrames
	{
		std::vector<NodeKeyFrameData> rotations;
		std::vector<NodeKeyFrameData> tranlations;
		std::vector<NodeKeyFrameData> scales;
	};

	struct GLTFAnimation
	{
		double duration;
		std::map<int, KeyFrames> data;

		inline NodeKeyFrameData* findKeyFrameTranslation(double time, int bone)
		{
			NodeKeyFrameData* frame = nullptr;
			if (data.find(bone) != data.end()) {
				for (auto& it : data[bone].tranlations) {
					if (it.time <= time) {
						frame = &it;
					}
				}
				if (frame == nullptr && data[bone].tranlations.size()) {
					frame = &data[bone].tranlations[data[bone].tranlations.size() - 1];
				}
			}

			return frame;
		}
		inline NodeKeyFrameData* findKeyFrameScale(double time, int bone)
		{
			NodeKeyFrameData* frame = nullptr;
			if (data.find(bone) != data.end()) {
				for (auto& it : data[bone].scales) {
					if (it.time <= time) {
						frame = &it;
					}
				}
				if (frame == nullptr && data[bone].scales.size()) {
					frame = &data[bone].scales[data[bone].scales.size() - 1];
				}
			}

			return frame;
		}
		inline NodeKeyFrameData* findKeyFrameRotation(double time, int bone)
		{
			NodeKeyFrameData* frame = nullptr;
			if (data.find(bone) != data.end()) {
				for (auto& it : data[bone].rotations) {
					if (it.time <= time) {
						frame = &it;
					}
				}
				if (frame == nullptr && data[bone].rotations.size()) {
					frame = &data[bone].rotations[data[bone].rotations.size() - 1];
				}
			}

			return frame;
		}
		inline NodeKeyFrameData* findKeyNextFrameTranslation(double time, int bone)
		{
			NodeKeyFrameData* frame = nullptr;

			if (data.find(bone) != data.end())
			{
				for (auto& it : data[bone].tranlations)
				{
					if (it.time > time) {
						frame = &it;
					}
				}
				if (frame == nullptr && data[bone].tranlations.size()) {
					frame = &data[bone].tranlations[data[bone].tranlations.size() - 1];
				}
			}

			return frame;
		}
		inline NodeKeyFrameData* findKeyNextFrameScale(double time, int bone)
		{
			NodeKeyFrameData* frame = nullptr;

			if (data.find(bone) != data.end())
			{
				for (auto& it : data[bone].scales)
				{
					if (it.time > time) {
						frame = &it;
					}
				}
				if (frame == nullptr && data[bone].scales.size()) {
					frame = &data[bone].scales[data[bone].scales.size() - 1];
				}
			}

			return frame;
		}
		inline NodeKeyFrameData* findKeyNextFrameRotation(double time, int bone)
		{
			NodeKeyFrameData* frame = nullptr;

			if (data.find(bone) != data.end())
			{
				for (auto& it : data[bone].rotations)
				{
					if (it.time > time) {
						frame = &it;
					}
				}
				if (frame == nullptr && data[bone].rotations.size()) {
					frame = &data[bone].rotations[data[bone].rotations.size() - 1];
				}
			}

			return frame;
		}
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
		std::vector<GLTFAnimation> gltfAnimations;
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
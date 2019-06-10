#pragma once

#include "tiny_gltf.h"
#include <fstream>
#include <unordered_map>
#include "Hollow/Graphics/Vertex.h"
#include <string>
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Quaternion.h"
#include "Hollow/Graphics/Vertex.h"

#define UNSIGNED_SHORT 5123
#define FLOAT 5126

namespace Hollow {
	struct NodeKeyFrameData
	{
		Vector3 vec3Val;
		Quaternion quatVal;
	};

	struct NodeAnimationData
	{
		int nodeId;
		std::unordered_map<float, NodeKeyFrameData> data;
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
		std::vector<NodeAnimationData> data;
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
	};

	struct LoadedMesh
	{
		std::vector<Vector3> normals;
		std::vector<Vector3> positions;
		std::vector<Vector2> texCoords;
		std::vector<float*> weigths;
		std::vector<unsigned short*> joints;
		std::vector<unsigned short> indices;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
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
		Node* rootNode;
	};

	struct Model
	{
		std::vector<LoadedMesh> meshes;
		std::unordered_map<std::string, Material> materials;
		std::vector<Animation> animations;
		Node* rootNode;
		AnimationNode* animationRootNode;
		std::vector<AnimationNode*> animationNodes;
	};
}

class GLTFImporter
{
public:
	Hollow::GLTFModel* import(const char* filename)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		Hollow::Model* lModel = new Hollow::Model();

		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

		tinygltf::Node& modelRootNode = model.nodes[model.scenes[0].nodes[0]];
		lModel->rootNode = new Hollow::Node(modelRootNode.name);

		std::ifstream file("Sandbox/Resources/Meshes/" + model.buffers[0].uri, std::fstream::in | std::fstream::binary);
		bool res = file.is_open();

		load(lModel->rootNode, modelRootNode, model, *lModel, file);
		processAnimation(*lModel, model, file);
		processSkin(*lModel, model, file);

		file.close();

		Hollow::GLTFModel* gltfModel = new Hollow::GLTFModel();
		gltfModel->rootNode = lModel->rootNode;
		gltfModel->meshes.resize(0);

		for (auto& model : lModel->meshes) {
			Hollow::Mesh* mesh = new Hollow::Mesh;

			for (int i = 0; i < model.positions.size(); i++) {
				Vertex vertex;

				vertex.pos = model.positions[i];

				if (model.normals.size() > i) {
					vertex.normal = model.normals[i];
				}
				if (model.texCoords.size() > i) {
					model.texCoords[i].y = 1.0f - model.texCoords[i].y;
					vertex.texCoord = model.texCoords[i];
				}

				mesh->vertices.push_back(vertex);
			}

			for (auto& it : model.indices) {
				mesh->indices.push_back(it);
			}

			gltfModel->meshes.push_back(mesh);
		}

		return gltfModel;
	}

	bool hasMesh(tinygltf::Node node, tinygltf::Model& model) {
		if (node.mesh >= 0) {
			return true;
		}

		if (node.children.size() == 0) {
			return false;
		}

		for (auto& child : node.children) {
			if (hasMesh(model.nodes[child], model)) {
				return true;
			}
		}
		return false;
	}

	void processMesh(Hollow::Node* node, tinygltf::Node childModelNode, Hollow::Model& model, tinygltf::Model& tModel, std::ifstream& file)
	{
		Hollow::LoadedMesh* mesh = new Hollow::LoadedMesh();

		// Poisitons and normals
		tinygltf::Mesh& tMesh = tModel.meshes[childModelNode.mesh];
		for (auto& it2 : tMesh.primitives[0].attributes) {
			tinygltf::Accessor& accessor = tModel.accessors[it2.second];
			tinygltf::BufferView& bufferView = tModel.bufferViews[accessor.bufferView];
			file.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

			if (accessor.componentType == FLOAT) {
				if (it2.first == "NORMAL") {
					float* data = new float[accessor.count * 3];

					file.read((char*)data, sizeof(float) * accessor.count * 3);

					for (int i = 0; i < accessor.count; i++) {
						mesh->normals.push_back(Vector3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]));
					}
				}
				else if (it2.first == "POSITION") {
					float* data = new float[accessor.count * 3];

					file.read((char*)data, sizeof(float) * accessor.count * 3);

					for (int i = 0; i < accessor.count; i++) {
						mesh->positions.push_back(Vector3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]));
					}
				}
				else if (it2.first == "WEIGHTS_0") {
					float* data = new float[accessor.count * 4];

					file.read((char*)data, sizeof(float) * accessor.count * 4);

					for (int i = 0; i < accessor.count; i++) {
						float* weights = new float[4]{ data[i] , data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3] };
						mesh->weigths.push_back(weights);
					}
				}
				else if (it2.first == "TEXCOORD_0") {
					float* data = new float[accessor.count * 2];

					file.read((char*)data, sizeof(float) * accessor.count * 2);

					for (int i = 0; i < accessor.count; i++) {
						mesh->texCoords.push_back(Vector2(data[i * 2], data[i * 2 + 1]));
					}
				}
			}
			else if (accessor.componentType == UNSIGNED_SHORT) {
				if (it2.first == "JOINTS_0") {
					unsigned short* data = new unsigned short[accessor.count * 4];

					file.seekg(accessor.byteOffset, std::fstream::cur);
					file.read((char*)data, sizeof(unsigned short) * accessor.count * 4);

					for (int i = 0; i < accessor.count; i++) {
						unsigned short* joints = new unsigned short[4]{ data[i] , data[i + 1], data[i + 2], data[i + 3] };
						mesh->joints.push_back(joints);
					}
				}
			}
		}

		// Indices
		tinygltf::Accessor& accessor = tModel.accessors[tMesh.primitives[0].indices];
		tinygltf::BufferView& bufferView = tModel.bufferViews[accessor.bufferView];
		file.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

		unsigned short* data = new unsigned short[accessor.count];

		file.read((char*)data, sizeof(unsigned short) * accessor.count);

		for (int i = 0; i < accessor.count; i++) {
			mesh->indices.push_back(data[i]);
		}
		model.meshes.push_back(*mesh);

		Hollow::Material lMaterial;
		tinygltf::Material& material = tModel.materials[tMesh.primitives[0].material];

		lMaterial.name = material.name;
		for (auto& values : material.values) {
			if (values.first == "baseColorTexture") {
				lMaterial.diffuseTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
			}
			else if (values.first == "metallicRoughnessTexture") {
				lMaterial.roughnesTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
			}
		}
		for (auto& values : material.additionalValues) {
			if (values.first == "emisiveTexture") {
				lMaterial.emisiveTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
			}
			else if (values.first == "normalTexture") {
				lMaterial.normalTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
			}
			else if (values.first == "occlusionTexture") {
				lMaterial.occlusionTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
			}
		}
		if (model.materials.find(lMaterial.name) == model.materials.end()) {
			model.materials[lMaterial.name] = lMaterial;
		}

		node->mesh = model.meshes.size() - 1;
	}

	void load(Hollow::Node * node, const tinygltf::Node & modelNode, tinygltf::Model & tModel, Hollow::Model & model, std::ifstream & file)
	{
		if (modelNode.matrix.data()) {
			node->transformation = Matrix4(modelNode.matrix.data(), 16);
		} else {
			node->transformation = Matrix4::Identity();
			if (modelNode.translation.size() > 0) {
				node->transformation = node->transformation *  Matrix4::Translation(Vector3(modelNode.translation[0], modelNode.translation[1], modelNode.translation[2]));
			}
			if (modelNode.scale.size() > 0) {
				node->transformation = node->transformation * Matrix4::Translation(Vector3(modelNode.scale[0], modelNode.scale[1], modelNode.scale[2]));
			}
			if (modelNode.rotation.size() > 0) {
				node->transformation = node->transformation * Quaternion(modelNode.rotation[0], modelNode.rotation[1], modelNode.rotation[2], modelNode.rotation[3]).toMatrix4();
			}
		}

		for (auto& it : modelNode.children) {
			tinygltf::Node& childModelNode = tModel.nodes[it];

			if (hasMesh(childModelNode, tModel)) {
				Hollow::Node* childNode = new Hollow::Node(childModelNode.name);

				load(childNode, childModelNode, tModel, model, file);
				node->childrens.push_back(childNode);

				if (childModelNode.mesh >= 0) {
					processMesh(childNode, childModelNode, model, tModel, file);
				}
			}
		}
	}

	void processAnimation(Hollow::Model & lModel, tinygltf::Model & model, std::ifstream & file)
	{
		// animation
		for (auto& animation : model.animations) {
			Hollow::Animation mAnimation;

			for (auto& channel : animation.channels) {
				tinygltf::AnimationSampler& sampler = animation.samplers[channel.sampler];
				tinygltf::Accessor& timeAccessor = model.accessors[sampler.input];
				tinygltf::BufferView& timeBufferView = model.bufferViews[timeAccessor.bufferView];
				file.seekg(timeBufferView.byteOffset + timeAccessor.byteOffset, std::fstream::beg);

				float* timeData = new float[timeAccessor.count];
				file.read((char*)timeData, sizeof(float) * timeAccessor.count);

				tinygltf::Accessor& valueAccessor = model.accessors[sampler.output];
				tinygltf::BufferView& valueBufferView = model.bufferViews[valueAccessor.bufferView];
				file.seekg(valueBufferView.byteOffset + valueAccessor.byteOffset, std::fstream::beg);

				float* valueData = nullptr;
				if (channel.target_path == "rotation") {
					valueData = new float[valueAccessor.count * 4];
					file.read((char*)valueData, sizeof(float) * valueAccessor.count * 4);
				}
				else {
					valueData = new float[valueAccessor.count * 3];
					file.read((char*)valueData, sizeof(float) * valueAccessor.count * 3);
				}

				Hollow::NodeAnimationData data;
				data.nodeId = channel.target_node;

				if (channel.target_path == "rotation") {
					for (int i = 0; i < timeAccessor.count; i++) {
						data.data[timeData[i]].quatVal = Quaternion(valueData[i * 4], valueData[i * 4 + 1], valueData[i * 4 + 2], valueData[i * 4 + 3]);
					}
				}
				else {
					for (int i = 0; i < timeAccessor.count; i++) {
						data.data[timeData[i]].vec3Val = Vector3(valueData[i * 3], valueData[i * 3 + 1], valueData[i * 3 + 2]);
					}
				}
				mAnimation.data.push_back(data);
			}
			lModel.animations.push_back(mAnimation);
		}
	}

	void processAnimationNode(Hollow::AnimationNode * node, const tinygltf::Node & modelNode, Hollow::Model & lModel, tinygltf::Model & model)
	{
		using namespace tinygltf;

		for (auto& child : modelNode.children) {
			Hollow::AnimationNode* animationNode = new Hollow::AnimationNode();
			Node& modelAnimationNode = model.nodes[child];
			animationNode->gltfId = child;

			node->childrens.push_back(animationNode);
			lModel.animationNodes.push_back(animationNode);
			processAnimationNode(animationNode, modelAnimationNode, lModel, model);
		}
	}

	void processSkin(Hollow::Model & lModel, tinygltf::Model & model, std::ifstream & file)
	{
		using namespace tinygltf;

		if (model.skins.size() > 0) {
			Node& rootNode = model.nodes[model.skins[0].skeleton];
			lModel.animationRootNode = new Hollow::AnimationNode();
			lModel.animationRootNode->gltfId = model.skins[0].skeleton;
			lModel.animationNodes.push_back(lModel.animationRootNode);

			Accessor& accessor = model.accessors[model.skins[0].inverseBindMatrices];
			BufferView& bufferView = model.bufferViews[accessor.bufferView];
			file.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

			Matrix4* matrixData = new Matrix4[model.skins[0].joints.size()];
			file.read((char*)matrixData, sizeof(float) * accessor.count * 16);

			processAnimationNode(lModel.animationRootNode, rootNode, lModel, model);

			for (int i = 0; i < model.skins[0].joints.size(); i++) {
				for (auto& node : lModel.animationNodes) {
					if (node->gltfId == model.skins[0].joints[i]) {
						node->id = i;
						node->localTransform = matrixData[i];
						break;
					}
				}
			}
		}
	}
};
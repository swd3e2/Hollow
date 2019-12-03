#pragma once
#include "Mesh.h"
#include <vector>
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Platform.h"
#include <string>
#include "Hollow/Math/Vector3.h"
#include "Hollow/Core/CModule.h"
#include "vendor/tinygltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include <fstream>
#include "Hollow/Common/Helper.h"

namespace Hollow {
	namespace Export {
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

		struct Node
		{
			int id;
			int mesh;
			int joint;
			std::string name;

			Vector3 translation;
			Quaternion rotation;
			Vector3 scale;

			std::vector<int> childs;
		};

		struct Material
		{
			int id;
			std::string name;

			Vector4 color;
			Vector3 emissiveColor;

			float metallic;
			float specular;
			float roughness;
			float ambientOcclusion;

			std::string diffuseTexture;
			std::string normalTexture;
			std::string specularTexture;
			std::string roughnesTexture;
			std::string emisiveTexture;
			std::string occlusionTexture;
			std::string metallicTexture;
		};

		struct Mesh
		{
			int id;
			int material;
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
		};

		struct Model
		{
			int rootNode;
			int rootJoint;
			std::vector<s_ptr<Animation>> animations;
			std::vector<s_ptr<Node>> nodes;
			std::vector<int> joints;
			std::vector<Matrix4> inverseBindMatrices;
			std::vector<s_ptr<Mesh>> meshes;
			std::vector<Material> materials;
		};
	}

	class HollowModelExporter : public CModule<HollowModelExporter>
	{
	public:
		void export2(const std::string& filename, const s_ptr<Export::Model>& model)
		{
			using json = nlohmann::json;

			std::string binaryFilename = Helper::trimToLastLineEntry(filename.c_str(), '.') + ".bin";
			std::ofstream bin(binaryFilename, std::ios::binary);

			json data;

			data["RootNode"] = model->rootNode;
			data["Skin"] = {
				{ "rootJoint", model->rootJoint },
				// offset will be here
				{ "inverseBindMatrices", 0 },
				{ "joints", model->joints }
			};

			data["Data"] = {
				{ "filename", filename },
				{ "size", 0 }
			};

			std::streampos indicesPos;
			std::streampos verticesPos;
			for (auto& mesh : model->meshes) {
				indicesPos = bin.tellp();
				bin.write((char*)mesh->indices.data(), sizeof(int)* mesh->indices.size());
				verticesPos = bin.tellp();
				bin.write((char*)mesh->vertices.data(), sizeof(Vertex) * mesh->vertices.size());

				json meshData = {
					{ "indices_offset", (int)indicesPos },
					{ "indices_size", mesh->indices.size() },
					{ "material", mesh->material },
					{ "vertices_offset", (int)verticesPos },
					{ "vertices_size", mesh->vertices.size() },
				};

				data["Meshes"].push_back(meshData);
			}

			for (auto& node : model->nodes) {
				json nodeData = {
					{ "id", node->id },
					{ "mesh", node->mesh },
					{ "joint", node->joint },
					{ "rotation", { node->rotation.x, node->rotation.y, node->rotation.z, node->rotation.w } },
					{ "translation", { node->translation.x, node->translation.y, node->translation.z } },
					{ "scale", { node->scale.x, node->scale.y, node->scale.z } },
					{ "childs", node->childs }
				};

				data["Nodes"].push_back(nodeData);
			}

			for (auto& material : model->materials) {
				json materialData = {
					{ "name", material.name },
					{ "base_color", { material.color.x, material.color.y, material.color.z, material.color.w } },
					{ "metallic", material.metallic },
					{ "roughness", material.roughness },
					{ "diffuse_texture", material.diffuseTexture },
					{ "emissive_texture", material.emisiveTexture },
					{ "normal_texture", material.normalTexture },
					{ "occlusion_texture", material.occlusionTexture },
					{ "roughness_texture", material.roughness },
					{ "specular_texture", material.specularTexture }
				};

				data["Materials"].push_back(materialData);
			}

			bin.close();
			Hollow::FileSystem::writeToFile(filename, data.dump(2).c_str());
		}
	};
}
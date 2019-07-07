#pragma once

#include "Mesh.h"
#include "vendor/tinygltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"

namespace Hollow {

	class HollowModelImporter
	{
	public:
		Import::Model* import(const char* filename)
		{
			using json = nlohmann::json;

			auto j = json::parse(FileSystem::getFileContent(filename));

			Import::Model* model = new Import::Model();

			std::string binFilename = j["Data"]["filename"].get<std::string>();

			const char* fileFolder = Helper::trim_to_last_line_entry(filename, '/');
			std::string folder;

			if (fileFolder) {
				folder = fileFolder;
			}

			std::ifstream file(folder + binFilename, std::ios::binary);

			for (int i = 0; i < j["Meshes"].size(); i++) {
				Import::Mesh* mesh = new Import::Mesh();
				mesh->material = j["Meshes"][i]["material"].get<int>();

				file.seekg(j["Meshes"][i]["vertices_offset"].get<size_t>(), std::fstream::beg);
				Hollow::Vertex* vertices = new Hollow::Vertex[j["Meshes"][i]["vertices_size"].get<size_t>()];
				file.read((char*)vertices, sizeof(Hollow::Vertex) * j["Meshes"][i]["vertices_size"].get<size_t>());

				mesh->vertices = std::vector<Vertex>(vertices, vertices + j["Meshes"][i]["vertices_size"].get<size_t>());

				file.seekg(j["Meshes"][i]["indices_offset"].get<size_t>(), std::fstream::beg);
				unsigned int* indices = new unsigned int[j["Meshes"][i]["indices_size"].get<size_t>()];
				file.read((char*)indices, sizeof(unsigned int) * j["Meshes"][i]["indices_size"].get<int>());

				mesh->indices = std::vector<unsigned int>(indices, indices + j["Meshes"][i]["indices_size"].get<int>());
				model->meshes.push_back(mesh);

				delete[] vertices;
				delete[] indices;
			}

			file.close();

			for (int i = 0; i < j["Materials"].size(); i++) {
				Import::Material material;
				material.name = j["Materials"][i]["name"].get<std::string>();
				material.baseColorFactor = Hollow::Vector4(
					j["Materials"][i]["base_color"][0].get<float>(),
					j["Materials"][i]["base_color"][1].get<float>(),
					j["Materials"][i]["base_color"][2].get<float>(),
					j["Materials"][i]["base_color"][3].get<float>()
				);

				material.metallicFactor = j["Materials"][i]["metallness"].get<float>();
				material.roughnessFactor = j["Materials"][i]["roughness"].get<float>();
				material.emissiveFactor = j["Materials"][i]["emissive"].get<float>();

				material.diffuseTexture = j["Materials"][i]["diffuse_texture"].get<std::string>();
				material.emisiveTexture = j["Materials"][i]["emissive_texture"].get<std::string>();
				material.normalTexture = j["Materials"][i]["normal_texture"].get<std::string>();
				material.occlusionTexture = j["Materials"][i]["occlusion_texture"].get<std::string>();
				material.roughnesTexture = j["Materials"][i]["roughness_texture"].get<std::string>();
				material.specularTexture = j["Materials"][i]["specular_texture"].get<std::string>();
				model->materials[j["Materials"][i]["id"].get<int>()] = material;
			}

			return model;
		}
	};

}
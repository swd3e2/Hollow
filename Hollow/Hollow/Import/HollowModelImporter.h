#pragma once

#include "Mesh.h"
#include "vendor/tinygltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include "Hollow/Platform.h"

namespace Hollow {

	class HollowModelImporter
	{
	public:
		s_ptr<Import::Model> import(const char* filename)
		{
			using json = nlohmann::json;

			auto j = json::parse(FileSystem::getFileContent(filename));

			s_ptr<Import::Model> model(new Import::Model());

			std::string binFilename = j["Data"]["filename"].get<std::string>();

			std::string fileFolder = Helper::trimToLastLineEntry(filename, '/');
			std::string folder;

			if (fileFolder.size()) {
				folder = fileFolder;
			}

			std::ifstream file(folder + binFilename, std::ios::binary);

			for (int i = 0; i < j["Meshes"].size(); i++) {
				s_ptr<Import::Mesh> mesh = std::make_shared<Import::Mesh>();
				mesh->material = j["Meshes"][i]["material"].get<int>();

				file.seekg(j["Meshes"][i]["vertices_offset"].get<size_t>(), std::fstream::beg);
				size_t verticesCount = j["Meshes"][i]["vertices_size"].get<size_t>();
				Hollow::Vertex* vertices = new Hollow::Vertex[verticesCount];
				file.read((char*)vertices, sizeof(Hollow::Vertex) * verticesCount);

				mesh->vertices = std::vector<Vertex>(vertices, vertices + verticesCount);

				file.seekg(j["Meshes"][i]["indices_offset"].get<size_t>(), std::fstream::beg);
				unsigned int* indices = new unsigned int[j["Meshes"][i]["indices_size"].get<size_t>()];
				file.read((char*)indices, sizeof(unsigned int) * j["Meshes"][i]["indices_size"].get<int>());

				mesh->indices = std::vector<unsigned int>(indices, indices + j["Meshes"][i]["indices_size"].get<int>());
				model->meshes.push_back(mesh);

				for (int i = 0; i < verticesCount; i++) {
					if (vertices[i].pos.x < model->A.x && vertices[i].pos.y < model->A.y && vertices[i].pos.z < model->A.z) {
						model->A = vertices[i].pos;
					}
					if (vertices[i].pos.x > model->B.x && vertices[i].pos.y > model->B.y && vertices[i].pos.z > model->B.z) {
						model->B = vertices[i].pos;
					}
				}

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
#include "Hollow/Importer/gltf/GLTFImporter.h"
#include "Hollow/Importer/gltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Graphics/Vertex.h"
#include <fstream>
#include "Hollow/Math/Matrix4.h"

using json = nlohmann::json;

int currentOffset = 0;

enum DataType {
	Vertex = 0,
	Index = 1
};

struct temp {
	int material;
	size_t vertices_offset;
	size_t indices_offset;
	size_t vertices_size;
	size_t indices_size;
};

void prepareModel(Hollow::GLTF::Node* node, const Hollow::Matrix4& parentTransform, Hollow::GLTF::GLTFModel* model)
{
	if (node->mesh != -1) {
		Hollow::Matrix4 transform = parentTransform * node->transformation;
		for (auto& it : model->meshes[node->mesh]->vertices) {
			it.pos = it.pos * transform;
			it.normal = it.normal * transform;
		}
	}
}

using namespace Hollow;

void saveGLTF()
{
	Hollow::GLTFImporter importer;
	Hollow::GLTF::GLTFModel* model = importer.import("scene.gltf");
	prepareModel(model->rootNode, Matrix4::Identity(), model);
	std::ofstream file("mesh1.bin", std::ios::binary);

	std::string file_content = Hollow::FileSystem::getFileContent("mesh1.json");

	int mesh_count = model->meshes.size();
	temp* temp_data = new temp[model->meshes.size()];
	json j;

	for (int i = 0; i < model->materials.size(); i++) {
		GLTF::Material& material = model->materials[i];
		j["Materials"][i] = {
			{"base_color", { material.baseColorFactor.x, material.baseColorFactor.y, material.baseColorFactor.z, material.baseColorFactor.w }},
			{"emissive", material.emissiveFactor},
			{"roughness", material.roughnessFactor},
			{"diffuse_texture", material.diffuseTexture},
			{"normal_texture", material.normalTexture},
			{"specular_texture", material.specularTexture},
			{"roughness_texture", material.roughnesTexture},
			{"emissive_texture", material.emisiveTexture},
			{"occlusion_texture", material.occlusionTexture}
		};
	}

	for (int i = 0; i < mesh_count; i++) {
		Hollow::GLTF::Mesh* mesh = model->meshes[i];
		// write vertex data
		file.write((char*)mesh->vertices.data(), mesh->vertices.size() * sizeof(Hollow::Vertex));
		temp_data[i].vertices_offset = currentOffset;
		temp_data[i].vertices_size = mesh->vertices.size();
		currentOffset += mesh->vertices.size() * sizeof(Hollow::Vertex);

		// write indices data
		file.write((char*)mesh->indices.data(), mesh->indices.size() * sizeof(unsigned int));
		currentOffset += mesh->indices.size() * sizeof(unsigned int);
		temp_data[i].indices_offset = currentOffset;
		temp_data[i].indices_size = mesh->indices.size();

		temp_data[i].material = mesh->material;
	}

	for (int i = 0; i < mesh_count; i++) {
		j["Meshes"][i] = {
			{"material", temp_data[i].material},
			{"vertices_size", temp_data[i].vertices_size},
			{"vertices_offset", temp_data[i].vertices_offset},
			{"indices_size", temp_data[i].indices_size},
			{"indices_offset", temp_data[i].indices_offset},
		};
	}

	j["Data"] = {
		{"filename", "mesh1.bin"},
		{"size", currentOffset}
	};

	file.close();
	std::string s = j.dump(2);

	Hollow::FileSystem::writeToFile("some.json", s.c_str());
}

int main() 
{
	saveGLTF();

	auto j = json::parse(FileSystem::getFileContent("some.json"));
	std::cout << j["Meshes"].size() << std::endl;

	std::string filename = j["Data"]["filename"].get<std::string>();
	std::ifstream file(filename, std::ios::binary);

	Hollow::Vertex** vertices = new Hollow::Vertex*[j["Meshes"].size()];

	int counter = 0;
	for (auto& it : j["Meshes"]) {
		file.seekg(it["vertices_offset"].get<size_t>(), std::fstream::beg);

		vertices[counter] = new Hollow::Vertex[it["vertices_size"].get<size_t>()];

		std::cout << it["vertices_offset"].get<size_t>() << std::endl;
		std::cout << it["vertices_size"].get<size_t>() << std::endl;

		file.read((char*)vertices[counter], sizeof(Hollow::Vertex) * it["vertices_size"].get<size_t>());
		counter++;
	}

	file.close();

	return 0;
}


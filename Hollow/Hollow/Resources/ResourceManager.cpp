#include "ResourceManager.h"

namespace Hollow {
	ResourceManager* ResourceManager::_instance = nullptr;

	ResourceManager::ResourceManager()
	{
		if (_instance == nullptr) {
			_instance = this;
		}
	}

	Mesh* ResourceManager::CreateMeshResource(std::string filename)
	{
		MeshData* data = objLoader.LoadObj(filename);
		Mesh* mesh = new Mesh(filename);
		
		if (data == nullptr) {
			return nullptr;
		}

		for (int i = 0; i < data->objects.size(); i++) {
			Containers::Vector<SimpleVertex>* vertices = new Containers::Vector<SimpleVertex>;

			for (int j = 0; j < data->objects[i]->indices.size(); j++) {

				SimpleVertex simpleVertex;
				simpleVertex.pos.x = data->vertices[data->objects[i]->indices[j].vertice_index * 3];
				simpleVertex.pos.y = data->vertices[1 + data->objects[i]->indices[j].vertice_index * 3];
				simpleVertex.pos.z = data->vertices[2 + data->objects[i]->indices[j].vertice_index * 3];

				if (data->objects[i]->has_texture) {
					simpleVertex.texCoord.x = -data->tex_coords[data->objects[i]->indices[j].tex_coord_index * 2];
					simpleVertex.texCoord.y = -data->tex_coords[1 + data->objects[i]->indices[j].tex_coord_index * 2];
				}
				else {
					simpleVertex.texCoord.x = 0.0f;
					simpleVertex.texCoord.y = 0.0f;
				}

				if (data->objects[i]->indices[j].normal_index > 0 && data->objects[i]->indices[j].normal_index * 3 + 2 < data->normals.size()) {
					simpleVertex.normal.x = data->normals[data->objects[i]->indices[j].normal_index * 3];
					simpleVertex.normal.y = data->normals[1 + data->objects[i]->indices[j].normal_index * 3];
					simpleVertex.normal.z = data->normals[2 + data->objects[i]->indices[j].normal_index * 3];
				}
				vertices->push_back(simpleVertex);
			}

			MeshModel* meshModel = new MeshModel(vertices->data(), vertices->size(), data->objects[i]->object_name);

			if (data->objects[i]->material != "" && data->hash_materials.find(data->objects[i]->material) != data->hash_materials.end()) {
				meshModel->material.name = data->objects[i]->material;
				meshModel->material.diffuse_texture = data->hash_materials[data->objects[i]->material]->diffuse_texture;
				meshModel->material.normal_texture = "dummy";
				meshModel->material.active = true;
			}

			mesh->objects.push_back(meshModel);
		}

		delete data;
		return mesh;
	}

	size_t ResourceManager::CreateTextureResource(std::string filename)
	{
		size_t textureId = 0;
		return textureId;
	}

	Sound* ResourceManager::CreateSoundResource(const char* filename)
	{
		/*if (this->sounds.find(filename) != this->sounds.end()) {
			return this->sounds[filename];
		}
		
		Sound* sound = this->m_AudioEngine.CreateSoundResource(filename);
		this->sounds[filename] = sound;

		return sound;*/
		return nullptr;
	}
}

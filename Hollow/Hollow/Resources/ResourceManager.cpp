#include "ResourceManager.h"

namespace Hollow {

	ResourceManager* ResourceManager::instance = nullptr;

	ResourceManager::ResourceManager() 
	{
		if (instance == nullptr)
			instance = this;
		else
			assert(true && "Trying to create more than 1 resource manager!");
	}

	Mesh * ResourceManager::CreateMeshResource(ID3D11Device * device, ID3D11DeviceContext* deviceContext, const char * filename, const char* mtl_base_dir)
	{
		MeshData* data = objLoader.LoadObj(filename, mtl_base_dir);
		Mesh* mesh = new Mesh();

		for (int i = 0; i < data->objects.size(); i++) {
			Containers::Vector<SimpleVertex> vertices;

			for (int j = 0; j < data->objects[i]->indices.size(); j++) {

				SimpleVertex simpleVertex;
				simpleVertex.pos.x = data->vertices[data->objects[i]->indices[j].vertice_index * 3];
				simpleVertex.pos.y = data->vertices[1 + data->objects[i]->indices[j].vertice_index * 3];
				simpleVertex.pos.z = data->vertices[2 + data->objects[i]->indices[j].vertice_index * 3];

				if (data->objects[i]->has_texture) {
					simpleVertex.texCoord.x = data->tex_coords[data->objects[i]->indices[j].tex_coord_index * 2];
					simpleVertex.texCoord.y = data->tex_coords[1 + data->objects[i]->indices[j].tex_coord_index * 2];
				} else {
					simpleVertex.texCoord.x = 0.0f; 
					simpleVertex.texCoord.y = 0.0f; 
				}
				
				simpleVertex.normal.x = data->normals[data->objects[i]->indices[j].normal_index * 3];
				simpleVertex.normal.y = data->normals[1 + data->objects[i]->indices[j].normal_index * 3];
				simpleVertex.normal.z = data->normals[2 + data->objects[i]->indices[j].normal_index * 3];

				vertices.push_back(simpleVertex);
			}
			
			MeshModel* meshModel = new MeshModel(device, vertices.data(), vertices.size());
			if (data->objects[i]->material != "" && data->hash_materials.find(data->objects[i]->material) != data->hash_materials.end()) {
				meshModel->material = new Material(data->objects[i]->material, CreateTextureResource(device, deviceContext, (char*)data->hash_materials[data->objects[i]->material]->diffuse_texture.c_str()));
			}

			mesh->objects.push_back(meshModel);
		}

		return mesh;
	}

	Texture * ResourceManager::CreateTextureResource(ID3D11Device * device, ID3D11DeviceContext * device_context, wchar_t * filename)
	{
		Texture* texture = new Texture(this->textureLoader.LoadTexture(device, device_context, filename));

		if (texture != nullptr) {
			_bstr_t convert(filename);
			char * charFileName = convert;
			Hollow::Log::GetCoreLogger()->critical("ResourceManager: can't load texture, filename: {}", charFileName);
		}
		return texture;
	}

	Texture * ResourceManager::CreateTextureResource(ID3D11Device* device, ID3D11DeviceContext* device_context, char* filename)
	{
		Texture* texture = new Texture(this->textureLoader.LoadTexture(device, device_context, filename));

		if (texture != nullptr) {
			_bstr_t convert(filename);
			char * charFileName = convert;
			Hollow::Log::GetCoreLogger()->critical("ResourceManager: can't load texture, filename: {}", charFileName);
		}
		return texture;
	}

	Material * ResourceManager::CreateMaterialResource(const char * filename)
	{
		return nullptr;
	}

	Sound * ResourceManager::CreateSoundResource(const char* filename)
	{
		if (this->sounds.find(filename) == this->sounds.end()) {
			Sound* sound = this->m_AudioEngine.CreateSoundResource(filename);
			this->sounds[filename] = sound;
		}
		return this->sounds[filename];
	}

}

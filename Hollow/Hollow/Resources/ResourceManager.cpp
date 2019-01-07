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

	Mesh * ResourceManager::CreateMeshResource(ID3D11Device * device, const char * filename, const char* mtl_base_dir)
	{
		MeshData* data = objLoader.LoadObj(filename, mtl_base_dir);
		Mesh* mesh = new Mesh();

		for (int i = 0; i < data->objects.size(); i++) {
			Hollow::Log::GetCoreLogger()->info("name object {}", data->objects[i]->object_name);
			Containers::Vector<SimpleVertex> vertices;

			for (int j = 0; j < data->objects[i]->indices.size(); j++) {
				SimpleVertex simpleVertex;
				simpleVertex.pos.x = data->vertices[data->objects[i]->indices[j].vertice_index * 3];
				simpleVertex.pos.y = data->vertices[1 + data->objects[i]->indices[j].vertice_index * 3];
				simpleVertex.pos.z = data->vertices[2 + data->objects[i]->indices[j].vertice_index * 3];

				simpleVertex.texCoord.x = 0.0f; //data->tex_coords[i * 3 + data->objects[i]->indices[j].tex_coord_index];
				simpleVertex.texCoord.y = 0.0f; //data->tex_coords[1 + i * 3 + data->objects[i]->indices[j].tex_coord_index];

				simpleVertex.normal.x = data->normals[i * 3 + data->objects[i]->indices[j].normal_index];
				simpleVertex.normal.y = data->normals[1 + i * 3 + data->objects[i]->indices[j].normal_index];
				simpleVertex.normal.z = data->normals[2 + i * 3 + data->objects[i]->indices[j].normal_index];

				vertices.push_back(simpleVertex);
			}
			
			MeshModel* meshModel = new MeshModel(device, vertices.data(), vertices.size());
			mesh->objects.push_back(meshModel);
		}

		return mesh;
	}

	Texture * ResourceManager::CreateTextureResource(ID3D11Device * device, ID3D11DeviceContext * device_context, const char * filename)
	{
		Texture* texture = new Texture();
		bool result = this->textureLoader.LoadFromWICFile(device, device_context, (wchar_t*)L"Sandbox/Resources/Textures/metal_texture.jpg", &texture->m_TextureShaderResource);

		result = 1;
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

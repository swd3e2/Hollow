#include "ResourceManager.h"

namespace Hollow {

	ResourceManager::ResourceManager() {}

	Mesh * ResourceManager::LoadFromObj(ID3D11Device * device, const char * filename)
	{
		MeshData* data = objLoader.LoadObj(filename);
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
				Hollow::Log::GetCoreLogger()->info("Position: x {} y {} z {}", simpleVertex.pos.x, simpleVertex.pos.y, simpleVertex.pos.z);

				vertices.push_back(simpleVertex);
			}
			
			MeshModel* meshModel = new MeshModel(device, vertices.data(), vertices.size());
			mesh->objects.push_back(meshModel);
		}

		return mesh;
	}

	SoundResource * ResourceManager::CreateSoundResource(const char* filename)
	{
		return this->m_AudioEngine.CreateSoundResource(filename);
	}

}

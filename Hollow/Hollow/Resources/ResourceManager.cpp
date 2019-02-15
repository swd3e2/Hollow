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

	Mesh * ResourceManager::CreateMeshResource(ID3D11Device * device, ID3D11DeviceContext* deviceContext, std::string filename, const char* mtl_base_dir)
	{
		if (this->meshes.find(filename) != this->meshes.end()) {
			return this->meshes[filename];
		}

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
					simpleVertex.texCoord.x = -data->tex_coords[data->objects[i]->indices[j].tex_coord_index * 2];
					simpleVertex.texCoord.y = -data->tex_coords[1 + data->objects[i]->indices[j].tex_coord_index * 2];
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
				meshModel->material = CreateMaterialResource(device, deviceContext, data->objects[i]->material, data->hash_materials[data->objects[i]->material]->diffuse_texture);
			} else {
				meshModel->material = new Material();
			}

			mesh->objects.push_back(meshModel);
		}

		this->meshes[filename] = mesh;
		return mesh;
	}

	Texture * ResourceManager::CreateTextureResource(ID3D11Device * device, ID3D11DeviceContext * device_context, wchar_t * filename)
	{
		_bstr_t convert(filename);
		char * charFileName = convert;
		return CreateTextureResource(device, device_context, charFileName);
	}

	Texture * ResourceManager::CreateTextureResource(ID3D11Device* device, ID3D11DeviceContext* device_context, std::string filename)
	{
		Hollow::Log::GetCoreLogger()->info("ResourceManager: trying to load texture, filename: {}", filename.c_str());

		if (this->texutres.find(filename) != this->texutres.end()) {
			return this->texutres[filename];
		}

		Texture* texture = new Texture(this->textureLoader.LoadTexture(device, device_context, (char*)filename.c_str()));

		if (texture != nullptr) {
			Hollow::Log::GetCoreLogger()->critical("ResourceManager: can't load texture, filename: {}", filename.c_str());
		}

		this->texutres[filename] = texture;
		return texture;
	}

	Material * ResourceManager::CreateMaterialResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string material_name, std::string diffuse_texture_filename)
	{
		if (this->materials.find(material_name) != this->materials.end()) {
			return this->materials[material_name];
		}
		Material* material = new Material(material_name, CreateTextureResource(device, deviceContext, diffuse_texture_filename.c_str()));
		if (material != nullptr) {
			this->materials[material_name] = material;
		}
		else {
			return nullptr;
		}

		return material;
	}

	PixelShader* ResourceManager::CreatePixelShader(ID3D11Device * device, std::string filename, std::string shader_name)
	{
		if (this->pixelShaders.find(shader_name) != this->pixelShaders.end()) {
			return this->pixelShaders[shader_name];
		}

		PixelShader* pixelShader = new PixelShader(device, Helper::converToWideChar((char*)filename.c_str()));
		if (pixelShader != nullptr) {
			this->pixelShaders[shader_name] = pixelShader;
		}
		else {
			Hollow::Log::GetCoreLogger()->critical("ResourceManager: can't create PS, filename: {}", filename.c_str());
			return nullptr;
		}
		return pixelShader;
	}

	VertexShader * ResourceManager::CreateVertexShader(ID3D11Device * device, std::string filename, std::string shader_name, D3D11_INPUT_ELEMENT_DESC* layout, UINT layoutSize)
	{
		if (this->vertexShaders.find(shader_name) != this->vertexShaders.end()) {
			return this->vertexShaders[shader_name];
		}

		VertexShader* vertexShader = new VertexShader(device, Helper::converToWideChar((char*)filename.c_str()), layout, layoutSize);
		if (vertexShader != nullptr) {
			this->vertexShaders[shader_name] = vertexShader;
		}
		else {
			Hollow::Log::GetCoreLogger()->critical("ResourceManager: can't create VS, filename: {}", filename.c_str());
			return nullptr;
		}

		return vertexShader;
	}

	Sound * ResourceManager::CreateSoundResource(const char* filename)
	{
		if (this->sounds.find(filename) != this->sounds.end()) {
			return this->sounds[filename];
		}
		
		Sound* sound = this->m_AudioEngine.CreateSoundResource(filename);
		this->sounds[filename] = sound;

		return sound;
	}


}

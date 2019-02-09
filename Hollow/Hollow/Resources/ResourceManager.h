#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Components/MeshComponent.h"
#include "Hollow/ECS/Entities/IEntity.h"
#include "Hollow/Common/Log.h"
#include "AudioEngine.h"
#include "Sound.h"
#include "Mesh.h"
#include <stdio.h>
#include "Utils/ObjParser.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"
#include "Material.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "Hollow/Graphics/Shaders/PixelShader.h"
#include "Hollow/Graphics/Shaders/VertexShader.h"
#include "Hollow/Utils/Helper.h"

namespace Hollow {
	class HOLLOW_API ResourceManager 
	{
	private:
		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Sound*> sounds;
		std::unordered_map<std::string, Texture*> texutres;
		std::unordered_map<std::string, Material*> materials;
		std::unordered_map<std::string, VertexShader*> vertexShaders;
		std::unordered_map<std::string, PixelShader*> pixelShaders;

		AudioEngine m_AudioEngine;
		ObjParser objLoader;
		TextureLoader textureLoader;
	private:
		static ResourceManager* instance;
	public:
		ResourceManager();
		Sound* CreateSoundResource(const char* filename);
		Mesh* CreateMeshResource(ID3D11Device * device, ID3D11DeviceContext * device_context, std::string filename, const char* mtl_base_dir);
		Texture* CreateTextureResource(ID3D11Device * device, ID3D11DeviceContext * device_context, wchar_t * filename);
		Texture* CreateTextureResource(ID3D11Device * device, ID3D11DeviceContext * device_context, std::string filename);
		Material* CreateMaterialResource(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string material_name, std::string diffuse_texture_filename);
		PixelShader* CreatePixelShader(ID3D11Device * device, std::string filename);
		VertexShader* CreateVertexShader(ID3D11Device * device, std::string filename, D3D11_INPUT_ELEMENT_DESC* layout, UINT layoutSize);

		inline PixelShader* GetPixelShader(const char* filename) { 
			if (this->pixelShaders.find(filename) != this->pixelShaders.end()) {
				return this->pixelShaders[filename];
			}
			return nullptr;
		}

		inline VertexShader* GetVertexShader(const char* filename)
		{
			if (this->vertexShaders.find(filename) != this->vertexShaders.end()) {
				return this->vertexShaders[filename];
			}
			return nullptr;
		}

		static inline ResourceManager* Get() { return instance; }
	};
}

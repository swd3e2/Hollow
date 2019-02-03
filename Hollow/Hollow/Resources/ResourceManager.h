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

namespace Hollow {
	class HOLLOW_API ResourceManager 
	{
	private:
		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Sound*> sounds;
		std::unordered_map<std::string, Texture*> texutres;
		std::unordered_map<std::string, Material*> materials;

		AudioEngine m_AudioEngine;
		ObjParser objLoader;
		TextureLoader textureLoader;
	private:
		static ResourceManager* instance;
	public:
		ResourceManager();
		Sound* CreateSoundResource(const char* filename);
		Mesh* CreateMeshResource(ID3D11Device * device, ID3D11DeviceContext * device_context, const char* filename, const char* mtl_base_dir);
		Texture* CreateTextureResource(ID3D11Device * device, ID3D11DeviceContext * device_context, wchar_t * filename);
		Texture* CreateTextureResource(ID3D11Device * device, ID3D11DeviceContext * device_context, char* filename);
		Material* CreateMaterialResource(const char* filename);

		static inline ResourceManager* Get() { return instance; }
	};
}

#pragma once
#include "Hollow/Platform.h"
#include "Hollow/ECS/Entities/IEntity.h"
#include "Hollow/Common/Log.h"
#include "AudioEngine.h"
#include "Sound.h"
#include <stdio.h>
#include "Utils/ObjParser.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Utils/Helper.h"
#include "Hollow/Graphics/RenderEngine.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Graphics/IMaterial.h"

namespace Hollow {
	class HOLLOW_API ResourceManager 
	{
	public:
		std::unordered_map<std::string, Sound*> sounds;
		//AudioEngine m_AudioEngine;
		ObjParser objLoader;
	private:
		static ResourceManager* _instance;
	public:
		ResourceManager();
		Sound* CreateSoundResource(const char* filename);
		size_t CreateMeshResource(std::string filename);
		size_t CreateTextureResource(std::string filename);

		static inline ResourceManager* instance() 
		{ 
			return _instance; 
		}
	};
}

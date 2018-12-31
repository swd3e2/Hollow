#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Components/MeshComponent.h"
#include "Hollow/ECS/Entities/IEntity.h"
#include "Hollow/Common/Log.h"
#include "AudioEngine.h"
//#include "tiny_obj_loader.h"
#include "SoundResource.h"

namespace Hollow {
	class HOLLOW_API ResourceManager
	{
	private:
		AudioEngine m_AudioEngine;
	public:
		ResourceManager();
		//void LoadFromObj(IEntity* entity, const char * filename);
		SoundResource* CreateSoundResource(const char* filename);
	};
}

#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Components/MeshComponent.h"
#include "Hollow/ECS/Entities/IEntity.h"
#include "Hollow/Common/Log.h"
#include "AudioEngine.h"
#include "SoundResource.h"
#include "Mesh.h"
#include <stdio.h>
#include "Utils/ObjParser.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"

namespace Hollow {
	class HOLLOW_API ResourceManager 
	{
	private:
		AudioEngine m_AudioEngine;
		ObjParser objLoader;
	public:
		ResourceManager();
		SoundResource* CreateSoundResource(const char* filename);
		Mesh* LoadFromObj(ID3D11Device * device, const char* filename);
	};
}

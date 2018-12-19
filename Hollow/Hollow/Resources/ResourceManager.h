#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Components/MeshComponent.h"
#include "Hollow/ECS/Entities/IEntity.h"
#include "Hollow/Common/Log.h"
#include "tiny_obj_loader.h"

namespace Hollow {
	class HOLLOW_API ResourceManager
	{
	public:
		ResourceManager();
		void LoadFromObj(IEntity* entity, const char * filename);
	};
}

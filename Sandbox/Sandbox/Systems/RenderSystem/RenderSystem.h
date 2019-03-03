#pragma once
#include "Hollow/ECS/Systems/System.h"
#include <windows.h>
#include <d3d11.h>
#include <directXMath.h>
#include <wrl/client.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Graphics/Camera.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/PositionComponent.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Hollow/Resources/ResourceManager.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Hollow/Containers/vector.h"

using namespace Hollow;

class RenderSystem : System<RenderSystem>
{
public:
	RenderSystem(int width, int height);

	virtual void PreUpdate(float_t dt) override;
	virtual void Update(float_t dt) override;
	virtual void PostUpdate(float_t dt) override;
};
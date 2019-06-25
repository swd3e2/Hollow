#pragma once

#include "Hollow/Graphics/Renderer/Base/RenderTarget.h"
#include "Hollow/Graphics/Camera.h"

struct Shadow
{
	Hollow::RenderTarget* renderTarget;
	Hollow::Camera* shadowCamera;
};
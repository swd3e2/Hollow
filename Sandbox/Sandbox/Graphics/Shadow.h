#pragma once

#include "Hollow/Graphics/Base/RenderTarget.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Math/Vector2.h"

struct Shadow
{
	Hollow::RenderTarget* renderTarget;
	Hollow::Camera* shadowCamera;
	float bias = 0.00001f;
	Hollow::Vector2 texelSize;
};
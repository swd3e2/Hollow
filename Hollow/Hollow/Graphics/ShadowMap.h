#pragma once
#include "Camera.h"
#include "DirectX/D3DRenderTarget.h"
#include "Camera.h"
#include "DirectX/D3DContext.h"

class ShadowMap
{
public:
	D3DRenderTarget shadowRenderTarget;
	Camera camera;
	float bias = 0.0023f;
public:
	ShadowMap(D3DContext& context, int width, int height) :
		shadowRenderTarget(context, width, height, RenderTargetType::SECONDARY, DXGI_FORMAT_R8G8B8A8_UNORM),
		camera(false)
	{
		camera.SetProjectionValues(75.0f, 1.0f, 0.1f, 10000.0f);
	}
};
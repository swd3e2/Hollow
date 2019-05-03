#pragma once
#include "Camera.h"
#include "Renderer/DirectX/D3D11RenderTarget.h"
#include "Camera.h"

class ShadowMap
{
public:
	D3D11RenderTarget shadowRenderTarget;
	Camera camera;
	float bias = 0.0023f;
public:
	ShadowMap(int width, int height) :
		shadowRenderTarget(width, height, RenderTargetType::SECONDARY, DXGI_FORMAT_R8G8B8A8_UNORM),
		camera(false)
	{
		camera.SetProjectionValues(75.0f, 1.0f, 0.1f, 10000.0f);
	}
};
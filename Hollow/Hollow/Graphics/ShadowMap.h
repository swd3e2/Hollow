#pragma once
#include "Camera.h"
#include "DirectX/D3DRenderTarget.h"

class ShadowMap
{
public:
	D3DRenderTarget shadowRenderTarget;
	Camera camera;
	float bias = 0.0023f;
public:
	ShadowMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int width, int height) : 
		shadowRenderTarget(device, deviceContext, width, height, RenderTargetType::SECONDARY, DXGI_FORMAT_R8G8B8A8_UNORM),
		camera(false)
	{
		camera.SetProjectionValues();
	}
};
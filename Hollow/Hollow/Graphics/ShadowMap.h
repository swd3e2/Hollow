#pragma once
#include "Camera.h"
#include "DirectX/D3DRenderTarget.h"
#include "MyCamera.h"

class ShadowMap
{
public:
	D3DRenderTarget shadowRenderTarget;
	MyCamera camera;
	float bias = 0.0023f;
public:
	ShadowMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int width, int height) : 
		shadowRenderTarget(device, deviceContext, width, height, RenderTargetType::SECONDARY, DXGI_FORMAT_R8G8B8A8_UNORM),
		camera(false)
	{
		camera.SetProjectionValues((float)Math::PI / 2.0f, 1.0f, 0.1f, 10000.0f);
	}
};
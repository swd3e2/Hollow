#pragma once

#ifndef HW_D3D_CONTEXT_H
#define HW_D3D_CONTEXT_H

#include "Hollow/Platform.h"
#include <d3d11.h>

class D3D11Context
{
public:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
public:
	~D3D11Context()
	{
		SAFE_RELEASE(device);
		SAFE_RELEASE(deviceContext);
		SAFE_RELEASE(swapChain);
	}
};

#endif
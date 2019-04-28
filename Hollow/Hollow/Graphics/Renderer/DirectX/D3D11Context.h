#pragma once

#ifndef HW_D3D11_CONTEXT_H
#define HW_D3D11_CONTEXT_H

#include "Hollow/Platform.h"
#include <d3d11.h>

class D3D11Context
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
public:
	D3D11Context(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain) :
		device(device), deviceContext(deviceContext), swapChain(swapChain)
	{}

	~D3D11Context()
	{
		SAFE_RELEASE(device);
		SAFE_RELEASE(deviceContext);
		SAFE_RELEASE(swapChain);
	}

	ID3D11Device* getDevice() const { return device; }
	ID3D11DeviceContext* getDeviceContext() const { return deviceContext; }
	IDXGISwapChain* getSwapChain() const { return swapChain; }
};

#endif
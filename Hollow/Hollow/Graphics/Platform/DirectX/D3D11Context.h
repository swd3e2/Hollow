#pragma once

#ifndef HW_D3D11_CONTEXT_H
#define HW_D3D11_CONTEXT_H

#include "Hollow/Platform.h"
#include <d3d11.h>

namespace Hollow {
	class D3D11Context
	{
	private:
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain* m_SwapChain;
	public:
		D3D11Context(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain) :
			m_Device(device), m_DeviceContext(deviceContext), m_SwapChain(swapChain)
		{}

		~D3D11Context()
		{
			SAFE_RELEASE(m_Device);
			SAFE_RELEASE(m_DeviceContext);
			SAFE_RELEASE(m_SwapChain);
		}

		inline ID3D11Device* getDevice() const { return m_Device; }
		inline ID3D11DeviceContext* getDeviceContext() const { return m_DeviceContext; }
		inline IDXGISwapChain* getSwapChain() const { return m_SwapChain; }
	};
}

#endif
#ifndef __CONSTANT_BUFFER_H__
#define __CONSTANT_BUFFER_H__
#include <wrl\client.h>
#include <d3d11.h>
#include "Platform.h"

namespace Hollow {
	template<class T>
	class HOLLOW_API ConstantBuffer
	{
	private:
		ConstantBuffer(const ConstantBuffer<T> &rhs);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		ID3D11DeviceContext * m_DeviceContext;
	public:
		ConstantBuffer() {}
		T data;
		ID3D11Buffer * Get() const
		{
			return m_Buffer.Get();
		}

		ID3D11Buffer * const* GetAddressOf() const
		{
			return m_Buffer.GetAddressOf();
		}

		HRESULT Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
		{
			m_DeviceContext = deviceContext;
			// Constant buffer
			D3D11_BUFFER_DESC constantBufferDesc;
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - sizeof(T) % 16)); // Align with 16 bytes
			constantBufferDesc.MiscFlags = 0;
			constantBufferDesc.StructureByteStride = 0;
			return device->CreateBuffer(&constantBufferDesc, NULL, m_Buffer.GetAddressOf());
		}

		bool Update()
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = m_DeviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				return false;
			}
			CopyMemory(mappedResource.pData, &data, sizeof(T));
			m_DeviceContext->Unmap(m_Buffer.Get(), 0);
			return true;
		}
	};
}

#endif

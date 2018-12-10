#ifndef __INDEX_BUFFER__
#define __INDEX_BUFFER__
#pragma once
#include <wrl\client.h>
#include <d3d11.h>

namespace Hollow {

	template<typename T>
	class IndexBuffer
	{
	public:
		T * BufferData() { return m_Data; }

		const UINT BufferSize() const { return m_numIndices; }

		void Init(ID3D11Device * device, T * data, UINT numIndicies)
		{
			if (m_pIndexBuffer.Get() != nullptr)
			{
				m_pIndexBuffer.Reset();
			}
			m_numIndices = numIndicies;
			D3D11_BUFFER_DESC indexBufferDesc;
			indexBufferDesc.ByteWidth = sizeof(T) * numIndicies;
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			m_Data = data;
			D3D11_SUBRESOURCE_DATA indexBufferData;
			indexBufferData.pSysMem = data;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;

			device->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				m_pIndexBuffer.GetAddressOf()
			);
		}

		ID3D11Buffer * Get() { return m_pIndexBuffer.Get(); }
	private:
		T * m_Data;
		UINT m_numIndices;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	};

}

#endif

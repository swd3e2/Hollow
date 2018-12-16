#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__
#include <wrl\client.h>
#include <d3d11.h>
#include "Common/Log.h"
#include "Platform.h"

namespace Hollow {

	template<typename T>
	class HOLLOW_API IndexBuffer
	{
	public:
		IndexBuffer(ID3D11Device * device, T * data, UINT numIndices)
		{
			HRESULT hr = S_OK;
			if (m_pIndexBuffer.Get() != nullptr)
			{
				m_pIndexBuffer.Reset();
			}
			m_numIndices = numIndices;
			D3D11_BUFFER_DESC indexBufferDesc;
			indexBufferDesc.ByteWidth = sizeof(T) * m_numIndices;
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA indexBufferData;
			indexBufferData.pSysMem = data;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;

			hr = device->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				m_pIndexBuffer.GetAddressOf()
			);
			if (hr != S_OK)
			{
				Hollow::Log::GetCoreLogger()->error("IndexBuffer: Cant create buffer!");
			}
		}
		const UINT BufferSize() const { return m_numIndices; }

		ID3D11Buffer * Get() { return m_pIndexBuffer.Get(); }
	private:
		UINT m_numIndices;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	};

}

#endif

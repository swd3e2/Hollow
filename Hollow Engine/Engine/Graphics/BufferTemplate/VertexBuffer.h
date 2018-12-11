#ifndef __VERTEX_BUFFER__
#define __VERTEX_BUFFER__
#pragma once
#include <wrl\client.h>
#include <d3d11.h>
#include <memory>
#include "Engine/Common/Log.h"

namespace Hollow {

	template <class T>
	class VertexBuffer
	{
	private:
		VertexBuffer(const VertexBuffer<T>& rhs);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		std::unique_ptr<UINT> stride;
		UINT bufferSize = 0;
	public:
		VertexBuffer(ID3D11Device * device, T * data, UINT numVertices) 
		{
			Init(device, data, numVertices);
		}

		ID3D11Buffer * Get() { return buffer.Get(); }
		ID3D11Buffer ** GetAddressOf() { return buffer.GetAddressOf(); }
		UINT BufferSize() const { return bufferSize; }
		T * BufferData() { return m_data; }
		const UINT Stride() const { return *stride.get(); }
		const UINT * StridePtr() const { return stride.get(); }
		T * m_data;

		void Init(ID3D11Device * device, T * data, UINT numVertices)
		{
			HRESULT hr = S_OK;

			if (buffer.Get() != nullptr)
			{
				buffer.Reset();
			}
			bufferSize = numVertices;
			if (stride.get() == nullptr)
			{
				stride = std::make_unique<UINT>(sizeof(T));
			}

			D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
			vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData;
			vertexBufferData.pSysMem = data;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;

			hr = device->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				buffer.GetAddressOf()
			);

			if (hr != S_OK)
			{
				Hollow::Log::GetCoreLogger()->error("IndexBuffer: Cant create buffer!");
			}
		}
	};

}

#endif

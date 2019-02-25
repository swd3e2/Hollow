#pragma once
#include <wrl\client.h>
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/IBuffer.h"

namespace Hollow {

	class D3DBuffer : public IBuffer
	{
	private:
		D3DBuffer(const D3DBuffer& rhs);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT stride;
		UINT bufferSize = 0;
	public:
		D3DBuffer(ID3D11Device* device, void* data, UINT stride, UINT numVertices, D3D11_BIND_FLAG bindFlag)
		{
			HRESULT hr = S_OK;

			if (buffer.Get() != nullptr)
			{
				buffer.Reset();
			}
			bufferSize = numVertices;

			D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
			vertexBufferDesc.ByteWidth = stride * numVertices;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = bindFlag; // D3D11_BIND_VERTEX_BUFFER; D3D11_BIND_INDEX_BUFFER
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData;
			vertexBufferData.pSysMem = data;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;

			hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf());

			if (hr != S_OK)
			{
				Hollow::Log::GetCoreLogger()->error("IndexBuffer: Cant create buffer!");
			}
		}

		ID3D11Buffer * Get() { return buffer.Get(); }
		ID3D11Buffer ** GetAddressOf() { return buffer.GetAddressOf(); }
		UINT BufferSize() const { return bufferSize; }
		const UINT Stride() const { return stride; }
		const UINT * StridePtr() const { return &stride; }
	};
}
#pragma once
#include <wrl\client.h>
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/IBuffer.h"

class HOLLOW_API D3DBuffer : public IBuffer
{
private:
	//D3DBuffer(const D3DBuffer& rhs);
private:
	ID3D11Buffer* buffer;
	UINT stride;
	UINT bufferSize = 0;
public:
	D3DBuffer(ID3D11Device* device, void* data, UINT stride, UINT numVertices, D3D11_BIND_FLAG bindFlag);
	ID3D11Buffer * Get() { return buffer; }
	ID3D11Buffer ** GetAddressOf() { return &buffer; }
	virtual size_t BufferSize() const override { return bufferSize; }
	const UINT Stride() const { return stride; }
	const UINT * StridePtr() const { return &stride; }
};

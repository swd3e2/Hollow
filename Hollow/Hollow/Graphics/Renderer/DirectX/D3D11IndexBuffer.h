#pragma once

#ifndef HW_D3D11_INDEX_BUFFER_H
#define HW_D3D11_INDEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include "D3D11Prerequisites.h"

class D3D11IndexBuffer : public IndexBuffer
{
private:
	ID3D11Buffer* buffer;
public:
	D3D11IndexBuffer(void* data, UINT stride, UINT numIndices);
	~D3D11IndexBuffer();
	
	ID3D11Buffer* Get() { return buffer; }
	ID3D11Buffer** GetAddressOf() { return &buffer; }
};

#endif
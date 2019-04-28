#pragma once

#ifndef HW_D3D11_VERTEX_BUFFER_H
#define HW_D3D11_VERTEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "D3D11Prerequisites.h"

class D3D11VertexBuffer : public VertexBuffer
{
private:
	ID3D11Buffer* buffer;
public:
	D3D11VertexBuffer(void* data, UINT stride, UINT numVertices);
	~D3D11VertexBuffer();
	
	ID3D11Buffer* Get() { return buffer; }
	ID3D11Buffer** GetAddressOf() { return &buffer; }
};

#endif
#include "D3D11HardwareBufferManager.h"

VertexBuffer* D3D11HardwareBufferManager::createVertexBuffer(Vertex* data, int numVertices)
{
	D3D11VertexBuffer* vBuffer = new D3D11VertexBuffer(data, sizeof(Vertex), numVertices);
	return vBuffer;
}

IndexBuffer* D3D11HardwareBufferManager::createIndexBuffer(unsigned int* data, int numIndices)
{
	D3D11IndexBuffer* iBuffer = new D3D11IndexBuffer(data, sizeof(unsigned int), numIndices);
	return iBuffer;
}

#pragma once
#include <DirectXMath.h>

struct SimpleVertex
{
	SimpleVertex() {}

	SimpleVertex(float px, float py, float pz, float tv, float tu, float nx, float ny, float nz)
		: pos(px, py, pz), texCoord(tv, tu), normal(nx, ny, nz)
	{}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
};
#pragma once
#include <d3d11.h>
#include <string>

class D3DTexture
{
public:
	D3DTexture(ID3D11ShaderResourceView* textureShaderResource) :
		m_TextureShaderResource(textureShaderResource)
	{}
	
	D3DTexture() {}

	ID3D11ShaderResourceView* m_TextureShaderResource;
};
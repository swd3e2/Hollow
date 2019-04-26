#pragma once
#include <d3d11.h>
#include <string>
#include "Hollow/Platform.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "D3D11RenderApi.h"

class D3D11Texture : public Texture
{
public:
	~D3D11Texture();

	void CreateTexture(std::string filename, int width, int height, void* data, int pitch);
	void SetShaderResource(ID3D11ShaderResourceView* textureShaderResource);
public:
	ID3D11ShaderResourceView* m_TextureShaderResource;
	bool active = false;
};
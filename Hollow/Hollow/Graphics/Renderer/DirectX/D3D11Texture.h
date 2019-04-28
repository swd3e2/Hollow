#pragma once

#ifndef HW_D3D11_TEXTURE_H
#define HW_D3D11_TEXTURE_H

#include <string>
#include "Hollow/Platform.h"
#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "D3D11Prerequisites.h"

class HOLLOW_API D3D11Texture : public Texture
{
public:
	~D3D11Texture();

	void CreateTexture(int width, int height, void* data, int pitch);
	void SetShaderResource(ID3D11ShaderResourceView* textureShaderResource);
public:
	ID3D11ShaderResourceView* m_TextureShaderResource;
	bool active = false;
};

#endif
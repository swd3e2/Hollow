#pragma once

#ifndef HW_D3D11_TEXTURE_H
#define HW_D3D11_TEXTURE_H

#include <string>
#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "D3D11Prerequisites.h"

class HOLLOW_API D3D11Texture : public Texture
{
public:
	~D3D11Texture();
	void SetShaderResource(ID3D11ShaderResourceView* textureShaderResource);
public:
	ID3D11UnorderedAccessView* m_UnorderedAccessView;
	ID3D11ShaderResourceView* m_TextureShaderResource;
	bool active = false;
};

#endif
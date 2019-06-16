#pragma once

#ifndef HW_D3D11_TEXTURE_H
#define HW_D3D11_TEXTURE_H

#include <string>
#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11Texture : public Texture
	{
	public:
		virtual ~D3D11Texture()
		{
			SAFE_RELEASE(m_TextureShaderResource);
			SAFE_RELEASE(m_UnorderedAccessView);
			SAFE_RELEASE(m_texture);
		}

		void SetShaderResource(ID3D11ShaderResourceView* textureShaderResource)
		{
			SAFE_RELEASE(m_TextureShaderResource);
			m_TextureShaderResource = textureShaderResource;
		}
	public:
		ID3D11UnorderedAccessView* m_UnorderedAccessView;
		ID3D11ShaderResourceView* m_TextureShaderResource;
		ID3D11Texture2D* m_texture;
		bool active = false;
	};
}

#endif
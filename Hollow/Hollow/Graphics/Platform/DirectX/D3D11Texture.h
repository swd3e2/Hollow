#pragma once

#ifndef HW_D3D11_TEXTURE_H
#define HW_D3D11_TEXTURE_H

#include <string>
#include "Hollow/Graphics/Texture.h"
#include "D3D11Prerequisites.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	class D3D11Texture : public Texture
	{
	public:	
		D3D11Texture(const TEXTURE_DESC& desc) :
			Texture(desc)
		{}

		virtual ~D3D11Texture()
		{
			SAFE_RELEASE(m_TextureShaderResource);
			SAFE_RELEASE(m_UnorderedAccessView);
			SAFE_RELEASE(m_Texture);
		}

		void SetShaderResource(ID3D11ShaderResourceView* textureShaderResource)
		{
			SAFE_RELEASE(m_TextureShaderResource);
			m_TextureShaderResource = textureShaderResource;
		}

		virtual void update(void* data) override
		{
			D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
			ID3D11Device* device = r->getContext().getDevice();
			ID3D11DeviceContext* deviceContext = r->getContext().getDeviceContext();

			D3D11_BOX box;
			box.left = 0;
			box.right = width;
			box.bottom = height;
			box.top = 0;
			box.front = 0;
			box.back = 1;

			if (type == TextureType::TT_TEXTURE2D) 
			{
				deviceContext->UpdateSubresource(m_Texture, 0, &box, data, D3D11Helper::getPicth(format, width), 1);
			}
			else if (type == TextureType::TT_TEXTURE_CUBE) 
			{
				unsigned char** texData = (unsigned char**)data;
				for (int face = 0; face < 6; face++) {
					deviceContext->UpdateSubresource(m_Texture, face * (numMips + 1), &box, texData[face] , D3D11Helper::getPicth(format, width), 0);
				}
			}
		}

		virtual void generateMipMap() override
		{
			D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
			ID3D11Device* device = r->getContext().getDevice();
			ID3D11DeviceContext* deviceContext = r->getContext().getDeviceContext();

			deviceContext->GenerateMips(m_TextureShaderResource);
		}
	public:
		ID3D11UnorderedAccessView* m_UnorderedAccessView = nullptr;
		ID3D11ShaderResourceView* m_TextureShaderResource = nullptr;
		ID3D11Texture2D* m_Texture = nullptr;
	};
}

#endif
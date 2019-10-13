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

			deviceContext->UpdateSubresource(m_Texture, 0, &box, data, D3D11Helper::getPicth(format, width), 1);
		}

	public:
		ID3D11UnorderedAccessView* m_UnorderedAccessView;
		ID3D11ShaderResourceView* m_TextureShaderResource;
		ID3D11Texture2D* m_Texture;
	};
}

#endif
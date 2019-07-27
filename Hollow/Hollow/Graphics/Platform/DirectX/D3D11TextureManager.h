#pragma once

#ifndef HW_D3D11_TEXTURE_MANAGER_H
#define HW_D3D11_TEXTURE_MANAGER_H

#include "D3D11Texture.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/TextureManager.h"
#include "D3D11Context.h"

namespace Hollow {
	class D3D11TextureManager : public TextureManager
	{
	public:
		virtual Texture* create2dTexture(TextureData* desc) override;
		virtual Texture* create3dTexture(TextureData** desc) override;
		virtual Texture* create3dTexture(TextureData* desc) override;
	private:
		DXGI_FORMAT getFormat(TextureFormat format);
	};
}

#endif
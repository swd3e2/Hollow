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
		virtual Texture* Create2dTexture(TEXTURE_DESC* desc) override;
		virtual Texture* Create3dTexture(TEXTURE_DESC** desc) override;
	private:
		DXGI_FORMAT getFormat(TextureFormat format);
	};
}

#endif
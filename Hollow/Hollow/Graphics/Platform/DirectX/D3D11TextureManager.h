#pragma once

#ifndef HW_D3D11_TEXTURE_MANAGER_H
#define HW_D3D11_TEXTURE_MANAGER_H

#include "D3D11Texture.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/TextureManager.h"
#include "D3D11Context.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Helper.h"

namespace Hollow {
	class D3D11TextureManager : public TextureManager
	{
	public:
		virtual s_ptr<Texture> create(const TEXTURE_DESC& desc) override;
		virtual s_ptr<Texture> create(const TEXTURE_DESC& desc, const s_ptr<Import::Texture>& texture) override;
	};
}

#endif
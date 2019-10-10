#pragma once

#ifndef HW_OGL_TEXTURE_MANAGER_H
#define HW_OGL_TEXTURE_MANAGER_H

#include "OGLPrerequisites.h"
#include "OGLTexture.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/TextureManager.h"

namespace Hollow {
	class OGLTextureManager : public TextureManager
	{
	public:
		virtual s_ptr<Texture> create2dTexture(const s_ptr<Import::Texture>& texture, const TEXTURE_DESC& desc) override;
		virtual s_ptr<Texture> create2dTexture(const TEXTURE_DESC& desc) override;
		virtual s_ptr<Texture> create3dTexture(const std::vector<s_ptr<Import::Texture>>& texture, const TEXTURE_DESC& desc) override;
		virtual s_ptr<Texture> create3dTexture(const s_ptr<Import::Texture>& texture, const TEXTURE_DESC& desc) override;
		virtual s_ptr<Texture> create3dTexture(const TEXTURE_DESC& desc) override;
	};
}

#endif
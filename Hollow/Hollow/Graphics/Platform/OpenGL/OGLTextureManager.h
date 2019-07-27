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
		virtual Texture* create2dTexture(TextureData* desc) override;
		virtual Texture* create3dTexture(TextureData** desc) override;
		virtual Texture* create3dTexture(TextureData* desc) override;
	};
}

#endif
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
		virtual Texture* create2dTexture(TEXTURE_DESC* desc) override;
		virtual Texture* create3dTexture(TEXTURE_DESC** desc) override;
		virtual Texture* create3dTexture(TEXTURE_DESC* desc) override;
	};
}

#endif
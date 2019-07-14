#pragma once

#ifndef HW_OGL_TEXTURE_H
#define HW_OGL_TEXTURE_H

#include "Hollow/Graphics/Base/Texture.h"

namespace Hollow {
	class OGLTexture : public Texture
	{
	public:
		unsigned int textureId;
	public:
		OGLTexture(int width, int height) : Texture(width, height), textureId(0) {}
		virtual ~OGLTexture()
		{
			glDeleteTextures(1, &textureId);
		}
	};
}

#endif
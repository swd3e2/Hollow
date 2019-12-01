#pragma once

#ifndef HW_OGL_TEXTURE_H
#define HW_OGL_TEXTURE_H

#include "Hollow/Graphics/Texture.h"
#include "OGLPrerequisites.h"
#include "OGLHelper.h"

namespace Hollow {
	class OGLTexture : public Texture
	{
	public:
		unsigned int textureId;
		GLuint textureTarget;
		GLuint texFormat;
	public:
		OGLTexture(const TEXTURE_DESC& desc) : 
			Texture(desc), textureId(0) 
		{}

		virtual ~OGLTexture()
		{
			glDeleteTextures(1, &textureId);
		}

		virtual void update(void* data) override
		{
			switch (type)
			{
			case Hollow::TT_TEXTURE1D:
				break;
			case Hollow::TT_TEXTURE2D: {
				glTextureSubImage2D(textureId, 0, 0, 0, width, height, 
					OGLHelper::getInternalTextureFormat(format), 
					OGLHelper::getInternalType(format), 
					data
				);
			} break;
			case Hollow::TT_TEXTURE3D:
				break;
			case Hollow::TT_TEXTURE_CUBE: {
				unsigned char** texData = (unsigned char**)data;
				for (int face = 0; face < 6; face++) {
					glTextureSubImage3D(textureId, 0, 0, 0, face, width, height, 1,
						OGLHelper::getInternalTextureFormat(format),
						OGLHelper::getInternalType(format),
						texData[face]
					);
				}
			} break;
				break;
			default:
				break;
			}
			
		}
		
		virtual void generateMipMap() override
		{
			glGenerateTextureMipmap(textureId);
		}
	};
}

#endif
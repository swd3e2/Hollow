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

		virtual void update(void* data) override
		{
			switch (type)
			{
			case Hollow::TT_TEXTURE1D:
				break;
			case Hollow::TT_TEXTURE2D: {
				glTextureSubImage2D(textureId,  // target
					0, // level
					0, // xoffset
					0, // yoffset
					width, // width
					height, // height
					OGLHelper::getInternalTextureFormat(format), // format
					OGLHelper::getInternalType(format), // type
					data // data
				);
			} break;
			case Hollow::TT_TEXTURE3D:
				break;
			case Hollow::TT_TEXTURE_CUBE: {
				glTextureSubImage2D(textureId,  // target
					0, // level
					0, // xoffset
					0, // yoffset
					width, // width
					height, // height
					OGLHelper::getInternalTextureFormat(format), // format
					OGLHelper::getInternalType(format), // type
					data // data
				);
			} break;
				break;
			default:
				break;
			}
			
		}

		virtual ~OGLTexture()
		{
			glDeleteTextures(1, &textureId);
		}
	};
}

#endif
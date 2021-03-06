#include "OGLTextureManager.h"

namespace Hollow {
	s_ptr<Texture> OGLTextureManager::create(const TEXTURE_DESC& desc)
	{
		OGLTexture* oglTexture = new OGLTexture(desc);
		oglTexture->type = desc.type;
		oglTexture->name = desc.name;
		oglTexture->depth = desc.depth;
		oglTexture->width = desc.width;
		oglTexture->height = desc.height;
		oglTexture->format = desc.format;
		oglTexture->numMips = desc.numMips;
		oglTexture->samples = desc.samples;

		GLuint numFaces = desc.format == TextureType::TT_TEXTURE_CUBE ? 6 : desc.arraySlices;
		oglTexture->textureTarget = OGLHelper::getTextureTarget(desc.type, desc.samples, numFaces);
		oglTexture->texFormat = OGLHelper::getTextureFormat(desc.format);

		int numMips = desc.numMips + 1;

		glCreateTextures(oglTexture->textureTarget, 1, &oglTexture->textureId);

		if (desc.samples <=	 1) {
			glTextureParameteri(oglTexture->textureId, GL_TEXTURE_BASE_LEVEL, 0);
			glTextureParameteri(oglTexture->textureId, GL_TEXTURE_MAX_LEVEL, numMips - 1);
		}

		switch (desc.type)
		{
		case TextureType::TT_TEXTURE1D:
		{
			if (numFaces <= 1) {
				glTextureStorage1D(oglTexture->textureId, numMips, oglTexture->texFormat, desc.width);
			} else {
				glTextureStorage2D(oglTexture->textureId, numMips, oglTexture->texFormat, desc.width, numFaces);
			}
		}
		break;
		case TextureType::TT_TEXTURE2D: {
			if (numFaces <= 1) {
				glTextureStorage2D(oglTexture->textureId, numMips, oglTexture->texFormat, desc.width, desc.height);
			} else {
				glTextureStorage3D(oglTexture->textureId, numMips, oglTexture->texFormat, desc.width, desc.height, numFaces);
			}
		}
		break;
		case TextureType::TT_TEXTURE3D:
			glTextureStorage3D(oglTexture->textureId, numMips, oglTexture->texFormat, desc.width, desc.height, desc.depth);
			break;
		case TextureType::TT_TEXTURE_CUBE: {
			if (numFaces <= 6) {
				glTextureStorage2D(oglTexture->textureId, numMips, oglTexture->texFormat, desc.width, desc.height);
			} else {
				glTextureStorage3D(oglTexture->textureId, numMips, oglTexture->texFormat, desc.width, desc.height, numFaces);
			}
		}
		break;
		}

		return s_ptr<Texture>(oglTexture);
	}

	s_ptr<Texture> OGLTextureManager::create(const TEXTURE_DESC& desc, const s_ptr<Import::Texture>& texture)
	{
		return s_ptr<Texture>();
	}
}
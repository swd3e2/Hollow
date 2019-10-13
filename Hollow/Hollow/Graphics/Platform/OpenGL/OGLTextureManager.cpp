#include "OGLTextureManager.h"

namespace Hollow {
	s_ptr<Texture> OGLTextureManager::create(const TEXTURE_DESC& desc)
	{
		/*
		int xOffset = texture->width / 4;
		int yOffset = texture->height / 3;
		std::array<unsigned char*, 6> data;
		for (int i = 0; i < data.size(); i++) {
			data[i] = new unsigned char[xOffset * yOffset * 4];
		}

		unsigned char* textureData = (unsigned char*)texture->data.get();

		OGLTexture* oglTexture = new OGLTexture(xOffset, yOffset);
		oglTexture->type = TextureType::TT_TEXTURE_CUBE;

		// forward 1 1
		// ? * xOffset * 4 - width | yOffset * desc->width * ? * 4 - plane
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[4][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[4][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[4][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[4][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// back  2 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[5][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[5][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[5][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[5][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// up 1 0
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[2][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 0];
				data[2][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 1];
				data[2][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 2];
				data[2][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// down 1 2
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[3][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 0];
				data[3][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 1];
				data[3][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 2];
				data[3][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// right 2 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[0][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[0][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[0][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[0][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// left 0 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[1][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[1][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[1][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[1][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}


		glGenTextures(1, &oglTexture->textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, oglTexture->textureId);
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, xOffset, yOffset, 0, GL_BGRA, GL_UNSIGNED_BYTE, data[i]);
		}
		*/

		OGLTexture* oglTexture = new OGLTexture(desc);

		GLuint numFaces = desc.format == TextureType::TT_TEXTURE_CUBE ? 6 : 1 * desc.arraySlices;
		oglTexture->textureTarget = OGLHelper::getTextureTarget(desc.type, desc.samples, numFaces);
		oglTexture->texFormat = OGLHelper::getTextureFormat(desc.format);

		int numMips = desc.numMips + 1;

		glCreateTextures(oglTexture->textureTarget, 1, &oglTexture->textureId);

		if (desc.samples <=	 1) {
			glTexParameteri(oglTexture->textureId, GL_TEXTURE_MAX_LEVEL, desc.numMips);
		}

		switch (desc.type)
		{
		case TextureType::TT_TEXTURE1D:
		{
			if (numFaces <= 1)
			{
				glTexStorage1D(GL_TEXTURE_1D, numMips, oglTexture->texFormat, desc.width);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_1D_ARRAY, numMips, oglTexture->texFormat, desc.width, numFaces);
			}
		}
		break;
		case TextureType::TT_TEXTURE2D:
		{
			if (numFaces <= 1)
			{
				glTexStorage2D(GL_TEXTURE_2D, numMips, oglTexture->texFormat, desc.width, desc.height);
			}
			else
			{
				glTexStorage3D(GL_TEXTURE_2D_ARRAY, numMips, oglTexture->texFormat, desc.width, desc.height, numFaces);
			}
		}
		break;
		case TextureType::TT_TEXTURE3D:
			glTexStorage3D(GL_TEXTURE_3D, numMips, oglTexture->texFormat, desc.width, desc.height, desc.depth);
			break;
		case TextureType::TT_TEXTURE_CUBE:
		{
			if (numFaces <= 6)
			{
				glTexStorage2D(GL_TEXTURE_CUBE_MAP, numMips, oglTexture->texFormat, desc.width, desc.height);
			}
			else
			{
				glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, numMips, oglTexture->texFormat, desc.width, desc.height, numFaces);
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
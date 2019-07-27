#include "OGLTextureManager.h"

namespace Hollow {
	Texture* OGLTextureManager::create2dTexture(TextureData* desc)
	{
		Hollow::Logger::instance()->log("D3D11TextureManager: creating 2d texture, filename {} bytes {}", desc->filename.c_str(), desc->size);
		HW_INFO("D3D11TextureManager: creating 2d texture, filename {} bytes {}", desc->filename.c_str(), desc->size);
		OGLTexture* texture = new OGLTexture(desc->width, desc->height);

		glGenTextures(1, &texture->textureId);
		glBindTexture(GL_TEXTURE_2D, texture->textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, desc->width, desc->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, *desc->data);

		glActiveTexture(GL_TEXTURE1);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		textureList[desc->filename] = texture;
		delete desc;

		return texture;
	}

	Texture* OGLTextureManager::create3dTexture(TextureData** desc)
	{
		Hollow::Logger::instance()->log("OGLTextureManager: creating 3d texture, filenames {} {} {} {} {} {} ", desc[0]->filename.c_str(), desc[1]->filename.c_str(), desc[2]->filename.c_str(), desc[3]->filename.c_str(), desc[4]->filename.c_str(), desc[5]->filename.c_str());
		HW_INFO("OGLTextureManager: creating 3d texture, filenames {} {} {} {} {} {} ", desc[0]->filename.c_str(), desc[1]->filename.c_str(), desc[2]->filename.c_str(), desc[3]->filename.c_str(), desc[4]->filename.c_str(), desc[5]->filename.c_str());
		OGLTexture* texture = new OGLTexture(desc[0]->width, desc[0]->height);
		texture->type = TextureType::TEXTURE_CUBE;

		glGenTextures(1, &texture->textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureId);
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, desc[i]->width, desc[i]->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, *desc[i]->data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		textureList[desc[0]->filename] = texture;

		for (int i = 0; i < 6; i++) {
			delete desc[i];
		}

		return texture;
	}
	Texture* OGLTextureManager::create3dTexture(TextureData* desc)
	{
		int xOffset = desc->width / 4;
		int yOffset = desc->height / 3;
		std::array<unsigned char*, 6> data;
		for (int i = 0; i < data.size(); i++) {
			data[i] = new unsigned char[xOffset * yOffset * 4];
		}

		unsigned char* textureData = (unsigned char*)*desc->data;

		OGLTexture* texture = new OGLTexture(xOffset, yOffset);
		texture->type = TextureType::TEXTURE_CUBE;

		// forward 1 1
		// ? * xOffset * 4 - width | yOffset * desc->width * ? * 4 - plane
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[4][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 0];
				data[4][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 1];
				data[4][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 2];
				data[4][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 3];
			}
		}

		// back  2 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[5][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 0];
				data[5][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 1];
				data[5][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 2];
				data[5][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 3];
			}
		}

		// up 1 0
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[2][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 0 * 4) + (oy * desc->width * 4) + 0];
				data[2][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 0 * 4) + (oy * desc->width * 4) + 1];
				data[2][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 0 * 4) + (oy * desc->width * 4) + 2];
				data[2][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 0 * 4) + (oy * desc->width * 4) + 3];
			}
		}

		// down 1 2
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[3][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 2 * 4) + (oy * desc->width * 4) + 0];
				data[3][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 2 * 4) + (oy * desc->width * 4) + 1];
				data[3][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 2 * 4) + (oy * desc->width * 4) + 2];
				data[3][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * desc->width * 2 * 4) + (oy * desc->width * 4) + 3];
			}
		}

		// right 2 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[0][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 0];
				data[0][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 1];
				data[0][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 2];
				data[0][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 3];
			}
		}

		// left 0 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[1][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 0];
				data[1][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 1];
				data[1][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 2];
				data[1][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * desc->width * 1 * 4) + (oy * desc->width * 4) + 3];
			}
		}


		glGenTextures(1, &texture->textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureId);
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, xOffset, yOffset, 0, GL_BGRA, GL_UNSIGNED_BYTE, data[i]);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		for (int i = 0; i < data.size(); i++) {
			delete[] data[i];
		}
		delete desc;

		return texture;
	}
}
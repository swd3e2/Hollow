#include "OGLTextureManager.h"

Texture* OGLTextureManager::Create2dTexture(TEXTURE_DESC* desc)
{
	HW_INFO("D3D11TextureManager: creating 2d texture, filename {} bytes {}", desc->filename.c_str(), desc->size);
	OGLTexture* texture = new OGLTexture(desc->width, desc->height);

	glGenTextures(1, &texture->textureId);
	glBindTexture(GL_TEXTURE_2D, texture->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, desc->width, desc->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, desc->mInitialData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	textureList[desc->filename] = texture;
	delete desc;

	return texture;
}

Texture* OGLTextureManager::Create3dTexture(TEXTURE_DESC** desc)
{
	HW_INFO("D3D11TextureManager: creating 3d texture, filenames {} {} {} {} {} {} ", desc[0]->filename.c_str(), desc[1]->filename.c_str(), desc[2]->filename.c_str(), desc[3]->filename.c_str(), desc[4]->filename.c_str(), desc[5]->filename.c_str());
	OGLTexture* texture = new OGLTexture(desc[0]->width, desc[0]->height);
	texture->type = TextureType::TEXTURE_CUBE;

	glGenTextures(1, &texture->textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureId);
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, desc[i]->width, desc[i]->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, desc[i]->mInitialData);
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

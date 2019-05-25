#include "OGLTextureManager.h"

Texture* OGLTextureManager::Create2dTexture(TEXTURE_DESC* desc)
{
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

	return texture;
}

Texture* OGLTextureManager::Create3dTexture(TEXTURE_DESC** desc)
{
	return nullptr;
}

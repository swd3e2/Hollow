#include "D3D11TextureManager.h"

Texture* D3D11TextureManager::CreateTextureInternal(TEXTURE_DESC* desc)
{
	D3D11Texture* texture = new D3D11Texture();

	texture->CreateTexture(desc->width, desc->height, desc->mInitialData, desc->pitch);
	textureList["0"] = texture;

	return texture;
}
#include "TextureManager.h"

TextureManager* TextureManager::_instance = nullptr;

D3DTexture * TextureManager::CreateTexture(std::string filename)
{
	if (textureList.find(filename) != textureList.end())
		return textureList[filename];
	
	D3DTexture* tex = new D3DTexture();
	tex->CreateTexture(device, deviceContext, filename.c_str());
	textureList[filename] = tex;

	return tex;
}
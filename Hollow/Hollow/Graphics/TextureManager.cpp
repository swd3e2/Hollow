#include "TextureManager.h"

D3DTexture * TextureManager::CreateTexture(std::string filename)
{
	Hollow::Log::GetCoreLogger()->critical("TextureManager: loading texture {}", filename);

	if (textureList.find(filename) != textureList.end())
		return textureList[filename];
	
	D3DTexture* tex = new D3DTexture();
	tex->CreateTexture(device, deviceContext, filename.c_str());
	textureList[filename] = tex;

	return tex;
}
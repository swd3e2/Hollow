#include "TextureManager.h"

TextureManager::TextureManager()
{
	setStartedUp();
}

TextureManager::~TextureManager()
{
	setShutdown();
}

Texture* TextureManager::CreateTextureFromFile(const std::string& filename)
{
	if (textureList.find(filename) != textureList.end()) {
		return textureList[filename];
	}

	std::string pathToFile = filename;
	pathToFile = baseTexturePapth + filename;

	TEXTURE_DESC* textureDesc = FreeImgImporter::instance()->import(pathToFile.c_str());

	if (textureDesc == nullptr) {
		return nullptr;
	}

	Texture* tex = Create2dTexture(textureDesc);

	textureList[filename] = tex;

	return tex;
}

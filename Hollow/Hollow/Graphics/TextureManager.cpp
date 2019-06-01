#include "TextureManager.h"

TextureManager::TextureManager()
{
	setStartedUp();
}

TextureManager::~TextureManager()
{
	setShutdown();
}

Texture* TextureManager::CreateTextureFromFile(const std::string& filename, bool fromDefaultFolder)
{
	if (textureList.find(filename) != textureList.end()) {
		return textureList[filename];
	}
	std::string pathToFile = filename;

	if (fromDefaultFolder) {
		pathToFile = baseTexturePapth + filename;
	}

	TEXTURE_DESC* textureDesc = FreeImgImporter::instance()->import(pathToFile.c_str());

	if (textureDesc == nullptr) {
		return nullptr;
	}

	Texture* tex = Create2dTexture(textureDesc);

	return tex;
}

void TextureManager::Remove(Texture* texture)
{
	for (auto& it : textureList) {
		if (it.second == texture) {
			textureList.erase(it.first);
			delete texture;
			break;
		}
	}
}

void TextureManager::RemoveAll()
{
	for (auto& it : textureList) {
		delete it.second;
		it.second = nullptr;
	}
}


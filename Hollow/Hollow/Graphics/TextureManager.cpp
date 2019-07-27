#include "TextureManager.h"

namespace Hollow {
	Texture* TextureManager::createTextureFromFile(const std::string& filename, bool fromDefaultFolder)
	{
		std::string pathToFile = filename;

		if (fromDefaultFolder) {
			pathToFile = baseTexturePapth + filename;
		}

		if (textureList.find(pathToFile) != textureList.end()) {
			return textureList[pathToFile];
		}

		TextureData* textureDesc = FreeImgImporter::instance()->import(pathToFile.c_str());

		if (textureDesc == nullptr) {
			return nullptr;
		}

		Texture* tex = create2dTexture(textureDesc);
		tex->name = filename;

		return tex;
	}

	void TextureManager::remove(Texture* texture)
	{
		for (auto& it : textureList) {
			if (it.second == texture) {
				textureList.erase(it.first);
				delete texture;
				break;
			}
		}
	}

	void TextureManager::removeAll()
	{
		for (auto& it : textureList) {
			delete it.second;
			it.second = nullptr;
		}
	}
}
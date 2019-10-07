#include "TextureManager.h"

namespace Hollow {
	s_ptr<Texture> TextureManager::createTextureFromFile(const std::string& filename, bool fromDefaultFolder)
	{
		std::string pathToFile = filename;

		if (fromDefaultFolder) {
			pathToFile = baseTexturePapth + filename;
		}

		if (textureList.find(pathToFile) != textureList.end()) {
			textureRefCnt[textureList[pathToFile].get()]++;
			return textureList[pathToFile];
		}

		s_ptr<TextureData> textureDesc = FreeImgImporter::instance()->import(pathToFile.c_str());

		if (textureDesc == nullptr) {
			return nullptr;
		}

		s_ptr<Texture> tex = create2dTexture(textureDesc);
		textureRefCnt[textureList[pathToFile].get()] = 1;

		tex->name = filename;

		return tex;
	}

	void TextureManager::remove(const s_ptr<Texture>& texture)
	{
		
	}

	void TextureManager::removeAll()
	{
		
	}
}
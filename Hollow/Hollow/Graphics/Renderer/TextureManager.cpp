#include "TextureManager.h"

Texture* TextureManager::CreateTextureFromFile(const char* filename)
{
	if (textureList.find(filename) != textureList.end()) {
		return textureList[filename];
	}

	std::string pathToFile = filename;
	pathToFile = baseTexturePapth + filename;

	TEXTURE_DESC* textureDesc = FreeImgImporter::import(filename);

	if (textureDesc == nullptr) {
		return nullptr;
	}

	Texture* tex = CreateTextureInternal(textureDesc);

	tex->CreateTexture(context, filename, data->width, data->height, data->data, data->pitch);
	textureList[filename] = tex;

	delete data;
	return tex;
}

#pragma once

#include "Hollow/Graphics/Texture.h"
#include <unordered_map>
#include "Hollow/Platform.h"
#include <Hollow/Core/CModule.h>
#include <algorithm>

class TextureManager : public Hollow::CModule<TextureManager>
{
public:
	std::unordered_map<std::string, Hollow::s_ptr<Hollow::Texture>> textureList;
public:
	Hollow::s_ptr<Hollow::Texture> create2DTextureFromFile(std::string filepath, int numMips = 0)
	{
		std::replace(filepath.begin(), filepath.end(), '\\', '/');

		if (Hollow::FileSystem::exists(filepath)) {
			if (textureList.find(filepath) != textureList.end()) {
				return textureList[filepath];
			}

			Hollow::s_ptr<Hollow::Import::Texture> textureData = Hollow::FreeImgImporter::instance()->import(filepath.c_str());

			if (textureData == nullptr) {
				HW_ERROR("Unable to load file, {}", filepath.c_str());
				return nullptr;
			}

			Hollow::TEXTURE_DESC desc;
			desc.width = textureData->width;
			desc.height = textureData->height;
			desc.format = textureData->bpp == 32 ? Hollow::TextureFormat::TF_BGRA8 : Hollow::TextureFormat::TF_BGR8;
			desc.type = Hollow::TextureType::TT_TEXTURE2D;
			desc.numMips = numMips;
			desc.name = filepath;

			Hollow::s_ptr<Hollow::Texture> texture = Hollow::Texture::create(desc);
			texture->update(textureData->data.get());

			if (numMips > 0) {
				texture->generateMipMap();
			}

			textureList[filepath] = texture;

			return texture;
		} else {
			HW_ERROR("Texture file not found, {}", filepath.c_str());
			return nullptr;
		}
	}

	bool remove(const Hollow::s_ptr<Hollow::Texture>& texture)
	{
		if (textureList.find(texture->name) != textureList.end()) {
			if (textureList[texture->name].use_count() < 2) {
				textureList.erase(texture->name);
			}
			return true;
		}

		return false;
	}
};
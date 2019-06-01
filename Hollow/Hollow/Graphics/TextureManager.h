#pragma once

#ifndef HW_TEXTURE_MANAGER_H
#define HW_TEXTURE_MANAGER_H

#include "Hollow/Common/Log.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Importer/FreeImgImporter.h"
#include "Renderer/Base/Texture.h"
#include <unordered_map>
#include <string>
#include "Hollow/Platform.h"

class TextureManager : public CModule<TextureManager>
{
public:
	std::unordered_map<std::string, Texture*> textureList;
	const std::string baseTexturePapth = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/";
public:
	TextureManager();
	~TextureManager();

	Texture* CreateTextureFromFile(const std::string& filename, bool fromDefaultFolder = true);
	void Remove(Texture* texture);
	void RemoveAll();
	virtual Texture* Create2dTexture(TEXTURE_DESC* desc) = 0;
	virtual Texture* Create3dTexture(TEXTURE_DESC** desc) = 0;
};

#endif
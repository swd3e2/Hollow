#pragma once

#ifndef HW_TEXTURE_MANAGER_H
#define HW_TEXTURE_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Hollow/Importer/FreeImgImporter.h"
#include "Renderer/Base/Texture.h"
#include <unordered_map>
#include <string>
#include "Hollow/Platform.h"

class HOLLOW_API TextureManager : public CModule<TextureManager>
{
public:
	std::unordered_map<std::string, Texture*> textureList;
	const std::string baseTexturePapth = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/";
public:
	TextureManager();
	~TextureManager();

	Texture* CreateTextureFromFile(const std::string& filename);

	virtual Texture* Create2dTexture(TEXTURE_DESC* desc) = 0;
	virtual Texture* Create3dTexture(TEXTURE_DESC** desc) = 0;
};

#endif
#pragma once

#ifndef HW_TEXTURE_MANAGER_H
#define HW_TEXTURE_MANAGER_H

#include "Hollow/Common/Log.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Import/Texture.h"
#include "Base/Texture.h"
#include <unordered_map>
#include <string>
#include "Hollow/Platform.h"

namespace Hollow {
	class TextureManager : public CModule<TextureManager>
	{
	public:
		std::unordered_map<std::string, s_ptr<Texture>> textureList;
		std::unordered_map<Texture*, size_t> textureRefCnt;
		const std::string baseTexturePapth = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/";
	public:
		s_ptr<Texture> createTextureFromFile(const std::string& filename, bool fromDefaultFolder = true);
		void remove(const s_ptr<Texture>& texture);
		void removeAll();
		virtual s_ptr<Texture> create2dTexture(const s_ptr<Import::Texture>& desc) = 0;
		virtual s_ptr<Texture> create3dTexture(const std::vector<s_ptr<Import::Texture>>& desc) = 0;
		virtual s_ptr<Texture> create3dTexture(const s_ptr<Import::Texture>& desc) = 0;
	};
}

#endif
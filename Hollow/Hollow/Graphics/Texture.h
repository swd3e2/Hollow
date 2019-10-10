#pragma once

#ifndef HW_TEXTURE_H
#define HW_TEXTURE_H

#include "Hollow/Graphics/CommonTypes.h"
#include "Hollow/Platform.h"
#include <string>
#include "Hollow/Common/Log.h"

namespace Hollow {
	struct TEXTURE_DESC
	{
		int width;
		int height;
		TextureFormat format;
		TextureType type;
		std::string name;
	};

	class TextureData
	{
	public:
		int width;
		int height;
		int pitch;
		int pixelSize;
		s_ptr<void> data;
		bool unorderedAccess;
		TextureFormat format;
		TextureType type;
		std::string filename;
		int size;
	public:
		TextureData() :
			format(FORMAT_B8G8R8A8_UNORM), unorderedAccess(false)
		{}
	};

	class Texture
	{
	public:
		int width;
		int height;
		TextureType type = TextureType::TEXTURE2D;
		std::string name;
	public:
		Texture(int width, int height) : width(width), height(height) {}
		Texture() : width(0), height(0) {}
		virtual ~Texture() {}
	};
}

#endif
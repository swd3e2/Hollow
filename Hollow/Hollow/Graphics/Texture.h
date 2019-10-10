#pragma once

#ifndef HW_TEXTURE_H
#define HW_TEXTURE_H

#include <memory>
#include "Hollow/Platform.h"
#include <string>
#include "Hollow/Common/Log.h"
#include "CommonTypes.h"

namespace Hollow {
	struct TEXTURE_DESC
	{
		int width;
		int height;
		TextureFormat format;
		TextureType type;
		std::string name;
	};

	class Texture
	{
	public:
		int width;
		int height;
		TextureType type = TextureType::TT_TEXTURE2D;
		std::string name;
	public:
		Texture(int width, int height) : width(width), height(height) {}
		Texture() : width(0), height(0) {}
		virtual ~Texture() {}
	};
}

#endif
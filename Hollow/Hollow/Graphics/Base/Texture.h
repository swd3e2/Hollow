#pragma once

#ifndef HW_TEXTURE_H
#define HW_TEXTURE_H

#include <memory>
#include "Hollow/Platform.h"
#include <string>

namespace Hollow {
	enum TextureFormat
	{
		FORMAT_B8G8R8A8_UNORM,
		FORMAT_R32G32B32A32
	};

	enum TextureType
	{
		TEXTURE2D,
		TEXTURE_CUBE
	};

	class TextureData
	{
	public:
		int width;
		int height;
		int pitch;
		int pixelSize;
		std::shared_ptr<void*> data;
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
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
		int width = 0;
		int height = 0;
		int depth = 0;
		int numMips = 0;
		int samples = 0;
		int arraySlices = 0;
		TextureFormat format = TextureFormat::TF_BGRA8;
		TextureType type = TextureType::TT_TEXTURE2D;
		std::string name;
	};

	class Texture
	{
	public:
		int width = 0;
		int height = 0;
		int depth= 0;
		int numMips = 0;
		int samples = 0;
		TextureType type;
		TextureFormat format;
		std::string name;
	public:
		Texture() = default;
		Texture(const TEXTURE_DESC& desc) :
			width(desc.width), height(desc.height), depth(desc.depth),
			numMips(desc.numMips), samples(desc.samples), format(desc.format),
			type(desc.type), name(desc.name)
		{}

		static s_ptr<Texture> create(const TEXTURE_DESC& desc);

		virtual void update(void* data) = 0;
		virtual ~Texture() {}
	};

	class Texture1D
	{

	};

	class Texture2D
	{

	};

	class Texutre3D
	{

	};

	class TextureCube
	{

	};
}

#endif
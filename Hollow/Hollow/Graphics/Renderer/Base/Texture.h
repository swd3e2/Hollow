#pragma once

#include <memory>
#include "Hollow/Platform.h"
#include <string>

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

class TEXTURE_DESC
{
public:
	int width;
	int height;
	int pitch;
	void* mInitialData;
	bool unorderedAccess;
	TextureFormat format;
	TextureType type;
	std::string filename;
	int size;
public:
	TEXTURE_DESC() :
		format(FORMAT_B8G8R8A8_UNORM), unorderedAccess(false)
	{}
	~TEXTURE_DESC()
	{
		if (mInitialData != nullptr) {
			free(mInitialData);
		}
	}
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
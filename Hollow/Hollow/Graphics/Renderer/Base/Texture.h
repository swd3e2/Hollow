#pragma once

#include <memory>
#include "Hollow/Platform.h"

enum TextureType
{
	TEXTURE2D,
	TEXTURE_CUBE
};

class HOLLOW_API TEXTURE_DESC
{
public:
	int width;
	int height;
	int pitch;
	void* mInitialData;
	bool unorderedAccess = false;
public:
	TEXTURE_DESC()
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

};
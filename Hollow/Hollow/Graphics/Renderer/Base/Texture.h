#pragma once

#include <memory>

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
public:
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
#pragma once

#include <memory>

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
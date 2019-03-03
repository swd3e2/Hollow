#pragma once
#include "FreeImage.h"
#include "Hollow/Platform.h"
#include "Hollow/Common/Log.h"

struct TextureData
{
	~TextureData()
	{
		if (data != nullptr) {
			Hollow::Log::GetClientLogger()->info("freed at address {}", data);
			free(data);
		}
	}
	int width;
	int height;
	int pitch;
	void* data;
};

class HOLLOW_API TextureLoader
{
public:
	static TextureData* loadFromFile(const char* filename)
	{
		TextureData* textureData = new TextureData();
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		BYTE* bits(0);

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return false;

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		//if the image failed to load, return failure
		if (!dib)
			return false;

		int BPP = FreeImage_GetBPP(dib);

		if (BPP != 32) {
			FIBITMAP* tmp = FreeImage_ConvertTo32Bits(dib);
			FreeImage_Unload(dib);
			dib = tmp;
		}

		FreeImage_FlipVertical(dib);

		//retrieve the image data
		bits = FreeImage_GetBits(dib);
		//get the image width and height
		textureData->width = FreeImage_GetWidth(dib);
		textureData->height = FreeImage_GetHeight(dib);
		textureData->pitch = FreeImage_GetPitch(dib);

		int size = textureData->width * textureData->height * 4;

		textureData->data = malloc(size);
		memcpy(textureData->data, bits, size);
		Hollow::Log::GetClientLogger()->info("allocated {} bytes at address {}", size, textureData->data);

		FreeImage_Unload(dib);

		//if this somehow one of these failed (they shouldn't), return failure
		if ((textureData->width == 0) || (textureData->height == 0))
			return nullptr;

		return textureData;
	}
};
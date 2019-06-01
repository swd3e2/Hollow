#include "FreeImgImporter.h"

TEXTURE_DESC* FreeImgImporter::import(const char* filename)
{
	TEXTURE_DESC* textureData = new TEXTURE_DESC();
	textureData->filename = filename;

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP* dib(0);
	//pointer to the image data
	BYTE* bits(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN) {
		//HW_ERROR("Texture manager: unknown file format, file {}", filename);
		return nullptr;
	}

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib) {
		//HW_ERROR("Texture manager: failed to load image, file {}", filename);
		return nullptr;
	}

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

	textureData->mInitialData = malloc(size);
	memcpy(textureData->mInitialData, bits, size);

	FreeImage_Unload(dib);

	//if this somehow one of these failed (they shouldn't), return failure
	if ((textureData->width == 0) || (textureData->height == 0)) {
		//HW_ERROR("Texture manager: file loaded with erorr's, can't get height and width, file {}", filename);
		return nullptr;
	}

	return textureData;
}

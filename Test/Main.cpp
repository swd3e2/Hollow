#pragma once
#include "FreeImage.h"
#include <iostream>

int main()
{
	const char* filename = "1.png";
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

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

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

	/*int BPP = FreeImage_GetBPP(dib);

	if (BPP != 32)
		dib = FreeImage_ConvertTo32Bits(dib);*/

	unsigned pitch = FreeImage_GetPitch(dib);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);

	if (image_type == FIT_RGBF) {
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (int y = 0; y < height; y++) {
			FIRGBF *pixel = (FIRGBF*)bits;
			for (int x = 0; x < width; x++) {
				pixel[x].red = 128;
				pixel[x].green = 128;
				pixel[x].blue = 128;
			}
			// next line
			bits += pitch;
		}
	}
	else if ((image_type == FIT_BITMAP) && (FreeImage_GetBPP(dib) == 24)) {
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (int y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for (int x = 0; x < width; x++) {
				std::cout << +pixel[FI_RGBA_RED] << " " << +pixel[FI_RGBA_GREEN] << " " << +pixel[FI_RGBA_BLUE] << " " << std::endl;
				pixel += 3;
			}
			// next line
			bits += pitch;
		}
	}



	return 0;
}


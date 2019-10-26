#include "FreeImgImporter.h"

namespace Hollow {
	s_ptr<Import::Texture> FreeImgImporter::import(const char* filename)
	{
		s_ptr<Import::Texture> texture = std::make_shared<Import::Texture>();
		texture->name = filename;

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
			HW_ERROR("Texture manager: unknown file format, file {}", filename);
			return nullptr;
		}

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		//if the image failed to load, return failure
		if (!dib) {
			HW_ERROR("Texture manager: failed to load image, file {}", filename);
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
		texture->width = FreeImage_GetWidth(dib);
		texture->height = FreeImage_GetHeight(dib);
		texture->pitch = FreeImage_GetPitch(dib);
		texture->bpp = FreeImage_GetBPP(dib);
		
		texture->size = texture->height * texture->pitch;

		texture->data = s_ptr<void>(malloc(texture->size), free);
		memcpy(texture->data.get(), bits, texture->size);

		FreeImage_Unload(dib);

		//if this somehow one of these failed (they shouldn't), return failure
		if ((texture->width == 0) || (texture->height == 0)) {
			HW_ERROR("Texture manager: file loaded with erorrs, can't get height and width, file {}", filename);
			return nullptr;
		}

		return texture;
	}
}
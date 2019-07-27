#pragma once

#ifndef HW_FREE_IMG_IMPORTER_H
#define HW_FREE_IMG_IMPORTER_H

#include "Hollow/Graphics/Base/Texture.h"
#include "Hollow/Platform.h"
#include "FreeImage.h"
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class FreeImgImporter : public CModule<FreeImgImporter>
	{
	public:
		virtual void onStartUp() override { FreeImage_Initialise(); }
		TextureData* import(const char* filename, bool converTo32Bits = true);
	};	
}

#endif
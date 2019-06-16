#pragma once

#ifndef HW_FREE_IMG_IMPORTER_H
#define HW_FREE_IMG_IMPORTER_H

#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "Hollow/Platform.h"
#include "FreeImage.h"
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class FreeImgImporter : public CModule<FreeImgImporter>
	{
	public:
		FreeImgImporter()
		{
			FreeImage_Initialise();
			setStartedUp();
		}
		~FreeImgImporter()
		{
			setShutdown();
		}

		TEXTURE_DESC* import(const char* filename);
	};
}

#endif
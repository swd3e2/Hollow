#pragma once

#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "Hollow/Platform.h"
#include "FreeImage.h"
#include "Hollow/Core/CModule.h"

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
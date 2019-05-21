#pragma once

#include "Hollow/Graphics/Renderer/Base/Texture.h"
#include "Hollow/Platform.h"
#include "FreeImage.h"
//#include "Hollow/Common/Log.h"

class  FreeImgImporter
{
public:
	static TEXTURE_DESC* import(const char* filename);
};
#pragma once

#ifndef HW_FREE_IMG_IMPORTER_H
#define HW_FREE_IMG_IMPORTER_H

#include "Hollow/Platform.h"
#include "FreeImage.h"
#include "Hollow/Core/CModule.h"
#include "Texture.h"
#include "Hollow/Common/Log.h"

namespace Hollow {
	class FreeImgImporter : public CModule<FreeImgImporter>
	{
	public:
		virtual void onStartUp() override { FreeImage_Initialise(); }
		s_ptr<Import::Texture> import(const char* filename);
	};	
}

#endif
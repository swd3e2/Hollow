#pragma once

#include <string>
#include "Hollow/Platform.h"

namespace Hollow {
	namespace Import
	{
		struct Texture
		{
			int width = 0;
			int height = 0;
			int pitch = 0;
			// bits per pixel
			int bpp = 0;
			int size = 0;
			std::string name;
			s_ptr<void> data;
		};
	}
}
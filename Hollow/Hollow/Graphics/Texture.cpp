#include "Texture.h"
#include "TextureManager.h"

namespace Hollow {
	s_ptr<Texture> Texture::create(const TEXTURE_DESC& desc)
	{
		return TextureManager::instance()->create(desc);
	}
}


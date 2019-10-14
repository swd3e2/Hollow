#pragma once

#ifndef HW_TEXTURE_MANAGER_H
#define HW_TEXTURE_MANAGER_H

#include "Hollow/Common/Log.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Import/Texture.h"
#include "Texture.h"
#include <unordered_map>
#include <string>
#include "Hollow/Platform.h"

namespace Hollow {
	class TextureManager : public CModule<TextureManager>
	{
	public:
		virtual s_ptr<Texture> create(const TEXTURE_DESC& desc) = 0;
		virtual s_ptr<Texture> create(const TEXTURE_DESC& desc, const s_ptr<Import::Texture>& texture) = 0;
	};
}

#endif
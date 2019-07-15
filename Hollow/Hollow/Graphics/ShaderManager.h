#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Base/Shader.h"
#include <unordered_map>
#include "Hollow/Platform.h"
#include "Base/ShaderProgram.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include <string>
#include "Hollow/Common/Logger.h"

namespace Hollow {
	class ShaderManager : public CModule<ShaderManager>
	{
	public:
		virtual Shader* create(const SHADER_DESC& desc) = 0;
	};
}
#endif
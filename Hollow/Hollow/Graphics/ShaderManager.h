#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Hollow/Common/Logger.h"
#include "Shader.h"
#include "ShaderPipeline.h"

namespace Hollow {
	class ShaderManager : public CModule<ShaderManager>
	{
	public:
		virtual s_ptr<Shader> create(const SHADER_DESC& desc) = 0;
		virtual s_ptr<ShaderPipeline> create(const SHADER_PIPELINE_DESC& desc) = 0;
	};
}
#endif
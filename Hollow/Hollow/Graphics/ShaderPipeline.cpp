#include "ShaderPipeline.h"
#include "ShaderManager.h"

namespace Hollow {
	s_ptr<ShaderPipeline> ShaderPipeline::create(const SHADER_PIPELINE_DESC& desc)
	{
		return ShaderManager::instance()->create(desc);
	}
}
#include "Shader.h"
#include "Hollow/Graphics/ShaderManager.h"

namespace Hollow {
	s_ptr<Shader> Shader::create(const SHADER_DESC& desc)
	{
		return ShaderManager::instance()->create(desc);
	}
}
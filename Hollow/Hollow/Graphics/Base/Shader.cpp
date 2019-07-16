#include "Shader.h"
#include "Hollow/Graphics/ShaderManager.h"

namespace Hollow {
	Shader* Shader::create(const SHADER_DESC& desc)
	{
		return ShaderManager::instance()->create(desc);
	}
}
#pragma once

#include "Hollow/Graphics/ShaderPipeline.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11ShaderPipeline : public ShaderPipeline
	{
	private:
		friend class D3D11ShaderManager;
	};
}
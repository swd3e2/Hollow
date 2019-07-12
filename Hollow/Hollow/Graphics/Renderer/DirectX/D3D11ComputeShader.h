#pragma once

#ifndef HW_D3D11_COMPUTE_SHADER_H
#define HW_D3D11_COMPUTE_SHADER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

namespace Hollow {
	class D3D11ComputeShader : public Shader
	{
	private:
		friend class D3D11ShaderManager;
	public:
		~D3D11ComputeShader()
		{
			release();
		}
		virtual void release() override
		{
			SAFE_RELEASE(m_Shader);
		}
		inline ID3D11ComputeShader* GetShader() const { return m_Shader; }
	private:
		ID3D11ComputeShader* m_Shader;
	};
}


#endif
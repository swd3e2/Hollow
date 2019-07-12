#pragma once

#ifndef HW_D3D11_PIXEL_SHADER_H
#define HW_D3D11_PIXEL_SHADER_H

#include <string>
#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

namespace Hollow {
	class D3D11PixelShader : public Shader
	{
	private:
		friend class D3D11ShaderManager;
	public:
		~D3D11PixelShader()
		{
			release();
		}
		virtual void release() override
		{
			SAFE_RELEASE(m_Shader);
		}
		inline ID3D11PixelShader* GetShader() { return m_Shader; }
	private:
		ID3D11PixelShader* m_Shader;
	};
}

#endif

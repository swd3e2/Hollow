#pragma once

#ifndef HW_D3D11_VERTEX_SHADER_H
#define HW_D3D11_VERTEX_SHADER_H

#include <wrl/client.h>
#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/Shader.h"

namespace Hollow{
	class D3D11VertexShader : public Shader
	{
	private:
		friend class D3D11ShaderManager;
	public:
		D3D11VertexShader(SHADER_TYPE type) : Shader(type) {}
		~D3D11VertexShader() { release(); }

		virtual void release() override { SAFE_RELEASE(m_Shader); }
		inline ID3D11VertexShader* getShader() const { return m_Shader; }
	private:
		ID3D11VertexShader* m_Shader;
	};
}

#endif
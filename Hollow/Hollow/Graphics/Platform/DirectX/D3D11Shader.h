#pragma once

#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11Shader : public Shader
	{
	private:
		friend class D3D11ShaderManager;
		ID3D11DeviceChild* m_Shader = nullptr;
	public:
		virtual ~D3D11Shader() { release(); }
		virtual void release() override { SAFE_RELEASE(m_Shader); }
		inline ID3D11DeviceChild* getShader() const { return m_Shader; }
	};
}
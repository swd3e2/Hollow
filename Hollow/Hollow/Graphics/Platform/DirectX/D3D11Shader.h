#pragma once

#include "D3D11Prerequisites.h"
#include <wrl/client.h>

namespace Hollow {
	class D3D11Shader : public Shader
	{
	private:
		friend class D3D11ShaderManager;
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> m_Shader;
	public:
		virtual void release() override
		{
			m_Shader->Release();
		}

		inline Microsoft::WRL::ComPtr<ID3D11DeviceChild> getShader() const
		{ 
			return m_Shader; 
		}
	};
}
#pragma once

#ifndef HW_D3D11_SHADER_MANAGER_H
#define HW_D3D11_SHADER_MANAGER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/ShaderManager.h"
#include <d3dcompiler.h>
#include "Hollow/Common/Log.h"
#include "D3D11Shader.h"
#include "Hollow/Platform.h"
#include "D3D11ShaderPipeline.h"

namespace Hollow {
	class D3D11ShaderManager : public ShaderManager
	{
	private:
		ID3D11Device* m_Device;
	public:
		D3D11ShaderManager();

		virtual s_ptr<Shader> create(const SHADER_DESC& desc) override;
		virtual s_ptr<ShaderPipeline> create(const SHADER_PIPELINE_DESC& desc) override;
		virtual void reload(const s_ptr<Shader>& shader, const std::string& path = "") override;
	private:
		HRESULT compileShaderInternal(const SHADER_DESC& desc, ID3DBlob** blob);
		HRESULT createShader(const SHADER_DESC& desc, ID3DBlob* blob, ID3D11DeviceChild** shader);
		const char* getTarget(const ShaderType type) const;
	};
}

#endif
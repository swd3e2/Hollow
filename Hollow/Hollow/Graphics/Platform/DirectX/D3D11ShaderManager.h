#pragma once

#ifndef HW_D3D11_SHADER_MANAGER_H
#define HW_D3D11_SHADER_MANAGER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/ShaderManager.h"
#include <d3dcompiler.h>
#include "D3D11ShaderProgram.h"
#include "Hollow/Common/Log.h"
	
namespace Hollow {
	class D3D11ShaderManager : public ShaderManager
	{
	private:
		ID3D11Device* device;
	public:
		D3D11ShaderManager();

		virtual Shader* create(const SHADER_DESC& desc) override;
		virtual ShaderProgram* createShaderProgram(const SHADER_PROGRAM_DESC& desc) override;
	private:
		HRESULT compileShaderInternal(const SHADER_DESC& desc, ID3DBlob** blob);
		const char* getTarget(const SHADER_TYPE& type);
	};
}

#endif
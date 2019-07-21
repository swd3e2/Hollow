#include "D3D11ShaderManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	D3D11ShaderManager::D3D11ShaderManager()
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		m_Device = r->getContext().getDevice();
	}

	Shader* D3D11ShaderManager::create(const SHADER_DESC& desc)
	{
		switch (desc.type)
		{
			case SHADER_TYPE::VERTEX: {
				D3D11VertexShader* shader = new D3D11VertexShader(SHADER_TYPE::VERTEX);
				ID3DBlob* shaderBlob = nullptr;

				if (!FAILED(compileShaderInternal(desc, &shaderBlob))) {
					m_Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader->m_Shader);
					shaderBlob->Release();
				}
				return shader;
			} break;
			case SHADER_TYPE::PIXEL: {
				D3D11PixelShader* shader = new D3D11PixelShader(SHADER_TYPE::PIXEL);
				ID3DBlob* shaderBlob = nullptr;

				if (!FAILED(compileShaderInternal(desc, &shaderBlob))) {
					m_Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader->m_Shader);
					shaderBlob->Release();
				}
				return shader;
			} break;
		default:
			break;
		}
	}

	HRESULT D3D11ShaderManager::compileShaderInternal(const SHADER_DESC& desc, ID3DBlob** blob)
	{
		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;

		const D3D_SHADER_MACRO defines[] = {
			{ "HLSL", "1" },
			{ nullptr, nullptr }
		};

		HRESULT hr = D3DCompile(desc.content.data(), desc.content.size(), NULL, defines, NULL, 
			desc.entryPoint.c_str(), getTarget(desc.type), D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &shaderBlob, &errorBlob);

		if (FAILED(hr)) {
			if (errorBlob) {
				HW_ERROR("{}", (char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (shaderBlob) {
				shaderBlob->Release();
			}

			return hr;
		}
		*blob = shaderBlob;

		return hr;
	}

	const char* D3D11ShaderManager::getTarget(const SHADER_TYPE& type)
	{
		switch (type)
		{
			case SHADER_TYPE::VERTEX:	return "vs_5_0";
			case SHADER_TYPE::PIXEL:	return "ps_5_0";
			case SHADER_TYPE::GEOMERTY:	return "gs_5_0";
			case SHADER_TYPE::COMPUTE:	return "cs_5_0";
			case SHADER_TYPE::HULL:		return "hs_5_0";
			case SHADER_TYPE::DOMAINS:	return "ds_5_0";
		}
	}
}


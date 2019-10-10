#include "D3D11ShaderManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	D3D11ShaderManager::D3D11ShaderManager()
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		m_Device = r->getContext().getDevice();
	}

	s_ptr<Shader> D3D11ShaderManager::create(const SHADER_DESC& desc)
	{
		D3D11Shader* shader = new D3D11Shader();
		ID3DBlob* shaderBlob = nullptr;

		if (!FAILED(compileShaderInternal(desc, &shaderBlob))) {
			createShader(desc, shaderBlob, shader);
			shaderBlob->Release();
		}

		return s_ptr<Shader>(shader);
	}

	s_ptr<ShaderPipeline> D3D11ShaderManager::create(const SHADER_PIPELINE_DESC& desc)
	{
		D3D11ShaderPipeline* shader = new D3D11ShaderPipeline();

		shader->vertexShader = desc.vertexShader;
		shader->pixelShader = desc.pixelShader;
		shader->geometryShader = desc.geometryShader;
		shader->hullShader = desc.hullShader;
		shader->domainShader = desc.domainShader;

		return s_ptr<ShaderPipeline>(shader);
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
			desc.entryPoint.c_str(), getTarget(desc.type), D3DCOMPILE_ENABLE_STRICTNESS,// | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
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

	HRESULT D3D11ShaderManager::createShader(const SHADER_DESC& desc, ID3DBlob* blob, const Microsoft::WRL::ComPtr<ID3D11DeviceChild>& shaderPtr)
	{
		switch (desc.type)
		{
		case ShaderType::ST_VERTEX:
			ID3D11VertexShader** shader = nullptr;
			shaderPtr->QueryInterface(shader);
			return m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);
		case ShaderType::ST_PIXEL: {
			ID3D11PixelShader** shader = nullptr;
			shaderPtr->QueryInterface(shader);
			return m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);
		} break;
		case ShaderType::ST_GEOMERTY: {
			ID3D11GeometryShader** shader = nullptr;
			shaderPtr->QueryInterface(shader);
			return m_Device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);
		} break;
		case ShaderType::ST_HULL: {
			ID3D11HullShader** shader = nullptr;
			shaderPtr->QueryInterface(shader);
			return m_Device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);
		} break;
		case ShaderType::ST_DOMAIN: {
			ID3D11DomainShader** shader = nullptr;
			shaderPtr->QueryInterface(shader);
			return m_Device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);
		} break;
		case ShaderType::ST_COMPUTE: {
			ID3D11ComputeShader** shader = nullptr;
			shaderPtr->QueryInterface(shader);
			return m_Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);
		} break;
		}
	}

	const char* D3D11ShaderManager::getTarget(const ShaderType type) const
	{
		switch (type)
		{
			case ShaderType::ST_VERTEX:		return "vs_5_0";
			case ShaderType::ST_PIXEL:		return "ps_5_0";
			case ShaderType::ST_GEOMERTY:	return "gs_5_0";
			case ShaderType::ST_COMPUTE:	return "cs_5_0";
			case ShaderType::ST_HULL:		return "hs_5_0";
			case ShaderType::ST_DOMAIN:		return "ds_5_0";
		}
	}
}


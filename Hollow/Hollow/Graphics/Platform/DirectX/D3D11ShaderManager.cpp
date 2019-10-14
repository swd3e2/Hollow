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

		if (!FAILED(compileShaderInternal(desc, &shaderBlob))) 
		{
			if (FAILED(createShader(desc, shaderBlob, &shader->m_Shader)))
			{
				HW_ERROR("Failed to create shader");
			}
			shaderBlob->Release();
		}
		else 
		{
			HW_ERROR("Failed to compile shader");
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

	HRESULT D3D11ShaderManager::createShader(const SHADER_DESC& desc, ID3DBlob* blob, ID3D11DeviceChild** shaderPtr)
	{
		switch (desc.type)
		{
		case ShaderType::ST_VERTEX: {
			return m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, (ID3D11VertexShader**)(shaderPtr));
		}
		case ShaderType::ST_PIXEL: {
			return m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, (ID3D11PixelShader * *)(shaderPtr));
		}
		case ShaderType::ST_GEOMERTY: {
			return m_Device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, (ID3D11GeometryShader * *)(shaderPtr));
		}
		case ShaderType::ST_HULL: {
			return m_Device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, (ID3D11HullShader * *)(shaderPtr));
		}
		case ShaderType::ST_DOMAIN: {
			return m_Device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, (ID3D11DomainShader * *)(shaderPtr));
		}
		case ShaderType::ST_COMPUTE: {
			return m_Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, (ID3D11ComputeShader * *)(shaderPtr));
		}
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


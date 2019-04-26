#pragma once
#include "D3D11IShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Hollow/Platform.h"
#include "D3D11RenderApi.h"

class D3D11VertexShader : public D3D11IShader
{
public:
	D3D11VertexShader(std::string filename)
	{
		HRESULT hr = CompileShader(filename, "VSMain", "vs_5_0", m_VertexShaderBlob.GetAddressOf());

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext()->device;

		hr = device->CreateVertexShader(m_VertexShaderBlob->GetBufferPointer(),
			m_VertexShaderBlob->GetBufferSize(), NULL, m_VertexShader.GetAddressOf());

		D3D11_INPUT_ELEMENT_DESC bxlayout[] =
		{
			{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,	 0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BITANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT numElements = ARRAYSIZE(bxlayout);

		// Create input layout to vertex shader
		hr = device->CreateInputLayout(bxlayout, numElements,
			m_VertexShaderBlob->GetBufferPointer(), m_VertexShaderBlob->GetBufferSize(),
			m_InputLayout.GetAddressOf());
	}

	ID3D11VertexShader * GetShader()
	{
		return m_VertexShader.Get();
	}

	ID3D11InputLayout * GetInputLayout()
	{
		return m_InputLayout.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> 	m_InputLayout;
	Microsoft::WRL::ComPtr<ID3DBlob> 			m_VertexShaderBlob;
};

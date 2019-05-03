#include "D3D11VertexShader.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

D3D11VertexShader::D3D11VertexShader(const std::string filename)
{
	HRESULT hr = CompileShader(filename, "VSMain", "vs_5_0", m_VertexShaderBlob.GetAddressOf());

	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	ID3D11Device* device = r->getContext().getDevice();

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

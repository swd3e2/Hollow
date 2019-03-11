#pragma once
#include "D3DIShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Hollow/Platform.h"

class D3DGeometryShader : D3DIShader
{
public:
	D3DGeometryShader(ID3D11Device * device, std::string filename)
	{
		HRESULT hr = CompileShader(filename, "main", "gs_5_0", m_VertexShaderBlob.GetAddressOf());

		hr = device->CreateGeometryShader(m_VertexShaderBlob->GetBufferPointer(),
			m_VertexShaderBlob->GetBufferSize(), NULL, m_GeomertryShader.GetAddressOf());
	}

	ID3D11GeometryShader * GetShader()
	{
		return m_GeomertryShader.Get();
	}

	ID3D11InputLayout * GetInputLayout()
	{
		return m_InputLayout.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>  m_GeomertryShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> 	  m_InputLayout;
	Microsoft::WRL::ComPtr<ID3DBlob> 			  m_VertexShaderBlob;
};

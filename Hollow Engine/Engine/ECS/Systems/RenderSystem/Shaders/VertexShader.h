
#ifndef __VERTEX_SHADER__
#define __VERTEX_SHADER__
#pragma once
#include "IShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

namespace Hollow {

	class VertexShader : IShader
	{
	public:
		void Init(ID3D11Device * device, std::wstring filename, D3D11_INPUT_ELEMENT_DESC * layoutDescription, UINT numElements)
		{
			HRESULT hr = CompileShader(filename.c_str(), "VSMain", "vs_5_0", m_VertexShaderBlob.GetAddressOf());

			hr = device->CreateVertexShader(m_VertexShaderBlob->GetBufferPointer(),
				m_VertexShaderBlob->GetBufferSize(), NULL, m_VertexShader.GetAddressOf());

			// Create input layout to vertex shader
			hr = device->CreateInputLayout(layoutDescription, numElements,
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

}

#endif
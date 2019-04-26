#pragma once
#include "D3D11IShader.h"
#include <d3d11.h>
#include <string>

class D3D11PixelShader : public D3D11IShader
{
public:
	D3D11PixelShader(ID3D11Device * device, std::string filename)
	{
		HRESULT hr = CompileShader(filename, "PSMain", "ps_5_0", &pixelShaderBlob);
		hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(), NULL, &pixelShader);
	}
	ID3D11PixelShader * GetShader()
	{
		return pixelShader;
	}
private:
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderBlob;
};

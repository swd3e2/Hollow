#pragma once
#include "D3DIShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Hollow/Platform.h"

class D3DPixelShader : D3DIShader
{
public:
	D3DPixelShader(ID3D11Device * device, std::string filename)
	{
		HRESULT hr = CompileShader(filename, "PSMain", "ps_5_0", pixelShaderBlob.GetAddressOf());

		hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(), NULL, pixelShader.GetAddressOf());
	}
	ID3D11PixelShader * GetShader()
	{
		return pixelShader.Get();
	}
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;
};

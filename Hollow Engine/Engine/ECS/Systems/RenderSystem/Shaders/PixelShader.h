
#ifndef __PIXEL_SHADER__
#define __PIXEL_SHADER__
#pragma once
#include "IShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

namespace Hollow {

#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "IShader.h"
#include <string>

	class PixelShader : IShader
	{
	public:
		PixelShader(ID3D11Device * device, std::wstring filename)
		{
			HRESULT hr = CompileShader(filename.c_str(), "PSMain", "ps_5_0", pixelShaderBlob.GetAddressOf());

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

}

#endif
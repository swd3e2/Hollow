#ifndef __PIXEL_SHADER_H__
#define __PIXEL_SHADER_H__
#include "IShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Platform.h"

namespace Hollow {

	class HOLLOW_API PixelShader : IShader
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
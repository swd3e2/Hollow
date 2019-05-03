#include "D3D11PixelShader.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

D3D11PixelShader::D3D11PixelShader(const std::string filename)
{
	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	ID3D11Device* device = r->getContext().getDevice();

	HRESULT hr = CompileShader(filename, "PSMain", "ps_5_0", &pixelShaderBlob);
	hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(), NULL, &pixelShader);
}

ID3D11PixelShader* D3D11PixelShader::GetShader()
{
	return pixelShader;
}

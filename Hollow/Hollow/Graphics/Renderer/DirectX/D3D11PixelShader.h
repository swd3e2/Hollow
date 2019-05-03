#pragma once

#ifndef HW_D3D11_PIXEL_SHADER_H
#define HW_D3D11_PIXEL_SHADER_H

#include "D3D11IShader.h"
#include <string>
#include "D3D11Prerequisites.h"

class HOLLOW_API D3D11PixelShader : public D3D11IShader
{
public:
	D3D11PixelShader(const std::string filename);
	ID3D11PixelShader* GetShader();
private:
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderBlob;
};

#endif

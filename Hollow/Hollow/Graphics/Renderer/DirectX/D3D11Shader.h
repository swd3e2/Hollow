#pragma once

#ifndef HW_D3D11_SHADER_H
#define HW_D3D11_SHADER_H

#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

class D3D11Shader : public Shader 
{
private:
	D3D11VertexShader* vShader;
	D3D11PixelShader* pShader;
public:
	D3D11Shader(D3D11VertexShader* vertexShader = nullptr, D3D11PixelShader* pixelShader = nullptr) :
		vShader(vertexShader), pShader(pixelShader)
	{}

	D3D11VertexShader* getVertexShader() { return vShader; }
	D3D11PixelShader* getPixelShader() { return pShader; }
};

#endif
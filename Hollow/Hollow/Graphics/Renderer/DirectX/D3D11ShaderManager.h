#pragma once

#ifndef HW_D3D11_SHADER_MANAGER_H
#define HW_D3D11_SHADER_MANAGER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/ShaderManager.h"
#include <d3dcompiler.h>
#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "D3D11ComputeShader.h"
#include "D3D11GeometryShader.h"
#include "D3D11HullShader.h"
#include "D3D11DomainShader.h"

class  D3D11ShaderManager : public ShaderManager
{
private:
	ID3D11Device* device;
private:
	HRESULT CompileShaderInternal(const std::string& path, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob);
public:
	D3D11ShaderManager();
	~D3D11ShaderManager();
	virtual Shader* compileShader(ShaderType type, const std::string& path) override;
	virtual ShaderProgram* createShader(Shader* vertexShader, Shader* pixelShader) override;
};

#endif
#pragma once

#ifndef HW_D3D11_GEOMETRY_SHADER_H
#define HW_D3D11_GEOMETRY_SHADER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

class D3D11GeometryShader : public Shader
{
private:
	friend class D3D11ShaderManager;
public:
	D3D11GeometryShader() :
		Shader(ShaderType::GEOMERTY)
	{}

	~D3D11GeometryShader() {
		SAFE_RELEASE(m_Shader);
		SAFE_RELEASE(m_ShaderBlob);
	}
	inline ID3D11GeometryShader* GetShader() { return m_Shader; }
private:
	ID3D11GeometryShader* m_Shader;
	ID3DBlob* m_ShaderBlob;
};

#endif
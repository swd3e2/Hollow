#pragma once

#include "Hollow/Graphics/Renderer/Base/ShaderProgram.h"
#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "D3D11HullShader.h"
#include "D3D11DomainShader.h"
#include "D3D11GeometryShader.h"
#include "D3D11ComputeShader.h"

class D3D11ShaderProgram : public ShaderProgram
{
public:
	D3D11ShaderProgram(Shader* vShader = nullptr, Shader* pShader = nullptr, Shader* gShader = nullptr, Shader* cShader = nullptr) :
		ShaderProgram(vShader, pShader, gShader, cShader)
	{}

	virtual void release() override
	{
		if (pixelShader != nullptr) {
			static_cast<D3D11PixelShader*>(pixelShader)->release();
		}
		if (vertexShader != nullptr) {
			static_cast<D3D11VertexShader*>(vertexShader)->release();
		}
		if (geometryShader != nullptr) {
			static_cast<D3D11GeometryShader*>(geometryShader)->release();
		}
		if (computeShader != nullptr) {
			static_cast<D3D11ComputeShader*>(computeShader)->release();
		}
		if (hullShader != nullptr) {
			static_cast<D3D11HullShader*>(hullShader)->release();
		}
		if (domainShader != nullptr) {
			static_cast<D3D11DomainShader*>(domainShader)->release();
		}
	}
};
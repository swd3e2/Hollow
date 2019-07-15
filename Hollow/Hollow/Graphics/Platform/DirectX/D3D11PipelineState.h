#pragma once

#include "Hollow/Graphics/Base/PipelineState.h"
#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "D3D11GeometryShader.h"
#include "D3D11DomainShader.h"
#include "D3D11HullShader.h"
#include "D3D11ComputeShader.h"

namespace Hollow {
	class D3D11PipelineState : public PipelineState
	{
	public:
		D3D11VertexShader* getVertexShader() { return dynamic_cast<D3D11VertexShader*>(this->vertexShader); }
		D3D11PixelShader* getPixelShader() { return dynamic_cast<D3D11PixelShader*>(this->pixelShader); }
		D3D11GeometryShader* getGeometryShader() { return dynamic_cast<D3D11GeometryShader*>(this->geometryShader); }
		D3D11DomainShader* getDomainShader() { return dynamic_cast<D3D11DomainShader*>(this->domainShader); }
		D3D11HullShader* getHullShader() { return dynamic_cast<D3D11HullShader*>(this->hullShader); }
		D3D11ComputeShader* getComputeShader() { return dynamic_cast<D3D11ComputeShader*>(this->computeShader); }
	};
}
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
		D3D11VertexShader* getVertexShader() 
		{ 
			if (this->vertexShader != nullptr) {
				return dynamic_cast<D3D11VertexShader*>(this->vertexShader);
			}
			return nullptr;
		}
		
		D3D11PixelShader* getPixelShader() 
		{ 
			if (this->pixelShader != nullptr) {
				return dynamic_cast<D3D11PixelShader*>(this->pixelShader);
			}
			return nullptr;
		}
		
		D3D11GeometryShader* getGeometryShader() 
		{ 
			if (this->geometryShader != nullptr) {
				return dynamic_cast<D3D11GeometryShader*>(this->geometryShader);
			}
			return nullptr;
		}
		
		D3D11DomainShader* getDomainShader() 
		{ 
			if (this->domainShader != nullptr) {
				return dynamic_cast<D3D11DomainShader*>(this->domainShader);
			}
			return nullptr;
		}
		
		D3D11HullShader* getHullShader() 
		{ 
			if (this->hullShader != nullptr) {
				return dynamic_cast<D3D11HullShader*>(this->hullShader);
			}
			return nullptr;
		}
		
		D3D11ComputeShader* getComputeShader() 
		{ 
			if (this->computeShader != nullptr) {
				return dynamic_cast<D3D11ComputeShader*>(this->computeShader);
			}
			return nullptr;
		}
	};
}
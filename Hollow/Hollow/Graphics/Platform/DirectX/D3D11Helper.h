#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/InputLayout.h"
#include "Hollow/Graphics/Base/HardwareBuffer.h"
#include <string>

namespace Hollow {
	class D3D11Helper 
	{
	public:
		static DXGI_FORMAT getInputLayoutFormat(INPUT_DATA_TYPE type)
		{
			DXGI_FORMAT result;

			switch (type)
			{
			case Hollow::Float:		
				result = DXGI_FORMAT_R32_FLOAT;
				break;
			case Hollow::Float2:
				result = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case Hollow::Float3:
				result = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case Hollow::Float4:
				result = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case Hollow::Int:
				result = DXGI_FORMAT_R32_SINT;
				break;
			case Hollow::Int2:
				result = DXGI_FORMAT_R32G32_SINT;
				break;
			case Hollow::Int3:
				result = DXGI_FORMAT_R32G32B32_SINT;
				break;
			case Hollow::Int4:
				result = DXGI_FORMAT_R32G32B32A32_SINT;
				break;
			case Hollow::Bool:
				result = DXGI_FORMAT_R32_UINT;
				break;
			}

			return result;
		}

		static std::string getInputLayoutShaderFormat(INPUT_DATA_TYPE type)
		{
			std::string result;

			switch (type)
			{
			case Hollow::Float:		
				result = "float";
				break;
			case Hollow::Float2:	
				result = "float2";
				break;
			case Hollow::Float3:
				result = "float3";
				break;
			case Hollow::Float4:
				result = "float4";
				break;
			case Hollow::Int:
				result = "int";
				break;
			case Hollow::Int2:
				result = "int2";
				break;
			case Hollow::Int3:
				result = "int3";
				break;
			case Hollow::Int4:
				result = "int4";
				break;
			case Hollow::Bool:
					result = "bool";
					break;
			}

			return result;
		}

		static unsigned int getSize(INDEX_FORMAT format)
		{
			unsigned int result = 0;

			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT:
			case Hollow::INDEX_FORMAT::INT:
				result = 4;
				break;
			}

			return result;
		}

		static D3D11_TEXTURE_ADDRESS_MODE getAddressingMode(const AddressingMode& mode)
		{
			D3D11_TEXTURE_ADDRESS_MODE res;

			switch (mode)
			{
			case WRAP:		
				res =  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
				break;
			case CLAMP:		
				res =  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
				break;
			case MIRROR:	
				res =  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
				break;
			case BORDER:	
				res =  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
				break;
			default:
					res = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			}

			return res;
		}

		static D3D11_COMPARISON_FUNC getComparisonFunction(const ComparisonFunction& func)
		{
			D3D11_COMPARISON_FUNC res;

			switch (func)
			{
			case NEVER:			
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
				break;
			case LESS:			
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
				break;
			case EQUAL:			
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL;
				break;
			case LEQUAL:		
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
				break;
			case GREATER:		
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
				break;
			case NOT_EQUAL:		
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL;
				break;
			default:
					res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
			}

			return res;
		}

		static D3D11_FILTER getFilteringMode(const FilterMode& min, const FilterMode& mag, const FilterMode& mip, const bool comparison)
		{
			D3D11_FILTER res;

#define MERGE_FILTERS(_comparison, _min, _mag, _mip) ((_comparison << 16) | (_min << 8) | (_mag << 4) | (_mip))
			switch ((MERGE_FILTERS(comparison, min, mag, mip))) 
			{
			case MERGE_FILTERS(true, POINT, POINT, POINT):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(true, POINT, POINT, LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, POINT, LINEAR, POINT):
				res = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(true, POINT, LINEAR, LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, LINEAR, POINT, POINT):
				res = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(true, LINEAR, POINT, LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, LINEAR, LINEAR, POINT):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(true, LINEAR, LINEAR, LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, ANISOTROPIC, ANISOTROPIC, ANISOTROPIC):
				res = D3D11_FILTER_COMPARISON_ANISOTROPIC;
				break;
			case MERGE_FILTERS(false, POINT, POINT, POINT):
				res = D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(false, POINT, POINT, LINEAR):
				res = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, POINT, LINEAR, POINT):
				res = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(false, POINT, LINEAR, LINEAR):
				res = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, LINEAR, POINT, POINT):
				res = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(false, LINEAR, POINT, LINEAR):
				res = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, LINEAR, LINEAR, POINT):
				res = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(false, LINEAR, LINEAR, LINEAR):
				res = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, ANISOTROPIC, ANISOTROPIC, ANISOTROPIC):
				res = D3D11_FILTER_ANISOTROPIC;
				break;
			default:
				res = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
#undef MERGE_FILTERS

			return res;
		}
	};
}
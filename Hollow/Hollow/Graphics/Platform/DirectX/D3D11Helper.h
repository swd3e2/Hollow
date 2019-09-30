#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/InputLayout.h"
#include "Hollow/Graphics/Base/HardwareBuffer.h"
#include "Hollow/Graphics/Base/CommonTypes.h"
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
			case AddressingMode::AM_WRAP:
				res =  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
				break;
			case AddressingMode::AM_CLAMP:
				res =  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
				break;
			case AddressingMode::AM_MIRROR:
				res =  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
				break;
			case AddressingMode::AM_BORDER:
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
			case ComparisonFunction::CMP_NEVER:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
				break;
			case ComparisonFunction::CMP_LESS:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
				break;
			case ComparisonFunction::CMP_EQUAL:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL;
				break;
			case ComparisonFunction::CMP_LEQUAL:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
				break;
			case ComparisonFunction::CMP_GREATER:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
				break;
			case ComparisonFunction::CMP_NOT_EQUAL:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL;
				break;
			case ComparisonFunction::CMP_ALWAYS:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
				break;
			default:
				res = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
			}

			return res;
		}

		static D3D11_FILTER getFilteringMode(const FilterMode& _min, const FilterMode& mag, const FilterMode& mip, const bool comparison)
		{
			D3D11_FILTER res;

#define MERGE_FILTERS(_comparison, _min, _mag, _mip) ((_comparison << 16) | (_min << 8) | (_mag << 4) | (_mip))
			switch ((MERGE_FILTERS((int)comparison, (int)_min, (int)mag, (int)mip)))
			{
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_POINT):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				res = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_POINT):
				res = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(true, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC):
				res = D3D11_FILTER_COMPARISON_ANISOTROPIC;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_POINT):
				res = D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				res = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_POINT):
				res = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				res = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				res = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;
			case MERGE_FILTERS(false, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC):
				res = D3D11_FILTER_ANISOTROPIC;
				break;
			default:
				res = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
#undef MERGE_FILTERS

			return res;
		}

		static D3D11_DEPTH_WRITE_MASK getDepthWriteMask(const DepthWriteMask& writeMask)
		{
			D3D11_DEPTH_WRITE_MASK mask = D3D11_DEPTH_WRITE_MASK_ALL;
			switch (writeMask)
			{
			case DepthWriteMask::DWM_ALL:
				mask = D3D11_DEPTH_WRITE_MASK_ALL;
				break;
			case DepthWriteMask::DWM_ZERO:
				mask = D3D11_DEPTH_WRITE_MASK_ZERO;
				break;
			}

			return mask;
		}

		static D3D11_STENCIL_OP getDepthStencilOperation(const StencilOperation& stecnilOp)
		{
			D3D11_STENCIL_OP res = D3D11_STENCIL_OP_KEEP;

			switch (stecnilOp)
			{
			case StencilOperation::SOP_KEEP:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
				break;
			case StencilOperation::SOP_ZERO:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO;
				break;
			case StencilOperation::SOP_INCR:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
				break;
			case StencilOperation::SOP_INCR_WRAP:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;
				break;
			case StencilOperation::SOP_DECR:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR;
				break;
			case StencilOperation::SOP_DECR_WRAP:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT;
				break;
			case StencilOperation::SOP_REPLACE:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE;
				break;
			case StencilOperation::SOP_INVERT:
				res = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT;
				break;
			default:
				break;
			}

			return res;
		}

		static D3D11_CULL_MODE getCullMode(const CullMode& cullMode)
		{
			D3D11_CULL_MODE res = D3D11_CULL_MODE::D3D11_CULL_NONE;

			switch (cullMode)
			{
			case CullMode::CLM_BACK:
				res = D3D11_CULL_MODE::D3D11_CULL_BACK;
				break;
			case CullMode::CLM_FRONT:
				res = D3D11_CULL_MODE::D3D11_CULL_FRONT;
				break;
			case CullMode::CLM_NONE:
				res = D3D11_CULL_MODE::D3D11_CULL_NONE;
				break;
			}

			return res;
		}

		static D3D11_FILL_MODE getFillMode(const FillMode& mode)
		{
			D3D11_FILL_MODE res = D3D11_FILL_MODE::D3D11_FILL_SOLID;

			switch (mode)
			{
			case FillMode::FM_SOLID:
				res = D3D11_FILL_MODE::D3D11_FILL_SOLID;
				break;
			case FillMode::FM_WIREFRAME:
				res = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
				break;
			}

			return res;
		}

		static D3D11_BLEND getBlend(const BlendFunction& blendFunc)
		{
			D3D11_BLEND res = D3D11_BLEND::D3D11_BLEND_ONE;

			switch (blendFunc)
			{
			case BlendFunction::BF_ZERO:
				res = D3D11_BLEND::D3D11_BLEND_ZERO;
				break;
			case BlendFunction::BF_ONE:
				res = D3D11_BLEND::D3D11_BLEND_ONE;
				break;
			case BlendFunction::BF_SRC_COLOR:
				res = D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
				break;
			case BlendFunction::BF_INV_SRC_COLOR:
				res = D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR;
				break;
			case BlendFunction::BF_SRC_APLHA:
				res = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
				break;
			case BlendFunction::BF_INV_SRC_APLHA:
				res = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
				break;
			case BlendFunction::BF_DEST_APLHA:
				res = D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
				break;
			case BlendFunction::BF_INV_DEST_APLHA:
				res = D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
				break;
			case BlendFunction::BF_DEST_COLOR:
				res = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
				break;
			case BlendFunction::BF_INV_DEST_COLOR:
				res = D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR;
				break;
			case BlendFunction::BF_BLEND_FACTOR:
				res = D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR;
				break;
			case BlendFunction::BF_INV_BLEND_FACTOR:
				res = D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR;
				break;
			}

			return res;
		}

		static D3D11_BLEND_OP getBlendOperation(const BlendOperation& blendOp)
		{
			D3D11_BLEND_OP res = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

			switch (blendOp)
			{
			case BlendOperation::BOP_ADD:
				res = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
				break;
			case BlendOperation::BOP_SUBTRACT:
				res = D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT;
				break;
			case BlendOperation::BOP_REV_SUBTRACT:
				res = D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT;
				break;
			case BlendOperation::BOP_MIN:
				res = D3D11_BLEND_OP::D3D11_BLEND_OP_MIN;
				break;
			case BlendOperation::BOP_MAX:
				res = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
				break;
			}

			return res;
		}
	};
}
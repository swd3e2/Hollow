#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/InputLayout.h"
#include "Hollow/Graphics/HardwareBuffer.h"
#include "Hollow/Graphics/CommonTypes.h"
#include <string>

namespace Hollow {
	class D3D11Helper 
	{
	public:
		static DXGI_FORMAT getInputLayoutFormat(INPUT_DATA_TYPE type)
		{
			switch (type)
			{
			case Hollow::Float:		
				return DXGI_FORMAT_R32_FLOAT;
			case Hollow::Float2:
				return DXGI_FORMAT_R32G32_FLOAT;
			case Hollow::Float3:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case Hollow::Float4:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case Hollow::Int:
				return DXGI_FORMAT_R32_SINT;
			case Hollow::Int2:
				return DXGI_FORMAT_R32G32_SINT;
			case Hollow::Int3:
				return DXGI_FORMAT_R32G32B32_SINT;
			case Hollow::Int4:
				return DXGI_FORMAT_R32G32B32A32_SINT;
			case Hollow::Bool:
				return DXGI_FORMAT_R32_UINT;
			}
		}

		static std::string getInputLayoutShaderFormat(INPUT_DATA_TYPE type)
		{
			switch (type)
			{
			case Hollow::Float:		
				return "float";
			case Hollow::Float2:	
				return "float2";
			case Hollow::Float3:
				return "float3";
			case Hollow::Float4:
				return "float4";
			case Hollow::Int:
				return "int";
			case Hollow::Int2:
				return "int2";
			case Hollow::Int3:
				return "int3";
			case Hollow::Int4:
				return "int4";
			case Hollow::Bool:
				return "bool";
			}
		}

		static unsigned int getSize(IndexFormat format)
		{
			switch (format)
			{
			case Hollow::IndexFormat::UINT:
			case Hollow::IndexFormat::INT:
				return 4;
			}
		}

		static D3D11_TEXTURE_ADDRESS_MODE getAddressingMode(const AddressingMode& mode)
		{
			switch (mode)
			{
			case AddressingMode::AM_WRAP:
				return  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			case AddressingMode::AM_CLAMP:
				return  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			case AddressingMode::AM_MIRROR:
				return  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
			case AddressingMode::AM_BORDER:
				return  D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
			default:
				return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}

		static D3D11_COMPARISON_FUNC getComparisonFunction(const ComparisonFunction& func)
		{
			switch (func)
			{
			case ComparisonFunction::CMP_NEVER:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
			case ComparisonFunction::CMP_LESS:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
			case ComparisonFunction::CMP_EQUAL:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL;
			case ComparisonFunction::CMP_LEQUAL:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
			case ComparisonFunction::CMP_GREATER:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
			case ComparisonFunction::CMP_NOT_EQUAL:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL;
			case ComparisonFunction::CMP_ALWAYS:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
			default:
				return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
			}
		}

		static D3D11_FILTER getFilteringMode(const FilterMode& _min, const FilterMode& mag, const FilterMode& mip, const bool comparison)
		{
#define MERGE_FILTERS(_comparison, _min, _mag, _mip) ((_comparison << 16) | (_min << 8) | (_mag << 4) | (_mip))
			switch ((MERGE_FILTERS((int)comparison, (int)_min, (int)mag, (int)mip)))
			{
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_POINT):
				return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case MERGE_FILTERS(true, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_POINT):
				return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			case MERGE_FILTERS(true, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			case MERGE_FILTERS(true, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC):
				return D3D11_FILTER_COMPARISON_ANISOTROPIC;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_POINT):
				return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case MERGE_FILTERS(false, FilterMode::FM_POINT, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_POINT):
				return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_POINT, FilterMode::FM_LINEAR):
				return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_POINT):
				return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case MERGE_FILTERS(false, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR, FilterMode::FM_LINEAR):
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			case MERGE_FILTERS(false, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC, FilterMode::FM_ANISOTROPIC):
				return D3D11_FILTER_ANISOTROPIC;
			default:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
#undef MERGE_FILTERS
		}

		static D3D11_DEPTH_WRITE_MASK getDepthWriteMask(const DepthWriteMask& writeMask)
		{
			switch (writeMask)
			{
			case DepthWriteMask::DWM_ALL:
				return D3D11_DEPTH_WRITE_MASK_ALL;
			case DepthWriteMask::DWM_ZERO:
				return D3D11_DEPTH_WRITE_MASK_ZERO;
			default:
				return D3D11_DEPTH_WRITE_MASK_ALL;
			}
		}

		static D3D11_STENCIL_OP getDepthStencilOperation(const StencilOperation& stecnilOp)
		{
			switch (stecnilOp)
			{
			case StencilOperation::SOP_KEEP:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
			case StencilOperation::SOP_ZERO:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO;
			case StencilOperation::SOP_INCR:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
			case StencilOperation::SOP_INCR_WRAP:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;
			case StencilOperation::SOP_DECR:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR;
			case StencilOperation::SOP_DECR_WRAP:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT;
			case StencilOperation::SOP_REPLACE:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE;
			case StencilOperation::SOP_INVERT:
				return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT;
			default:
				return D3D11_STENCIL_OP_KEEP;
			}
		}

		static D3D11_CULL_MODE getCullMode(const CullMode& cullMode)
		{
			switch (cullMode)
			{
			case CullMode::CLM_BACK:
				return D3D11_CULL_MODE::D3D11_CULL_BACK;
			case CullMode::CLM_FRONT:
				return D3D11_CULL_MODE::D3D11_CULL_FRONT;
			case CullMode::CLM_NONE:
				return D3D11_CULL_MODE::D3D11_CULL_NONE;
			}
		}

		static D3D11_FILL_MODE getFillMode(const FillMode& mode)
		{
			switch (mode)
			{
			case FillMode::FM_SOLID:
				return D3D11_FILL_MODE::D3D11_FILL_SOLID;
			case FillMode::FM_WIREFRAME:
				return D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
			default:
				return D3D11_FILL_MODE::D3D11_FILL_SOLID;
			}
		}

		static D3D11_BLEND getBlend(const BlendFunction& blendFunc)
		{
			switch (blendFunc)
			{
			case BlendFunction::BF_ZERO:
				return D3D11_BLEND::D3D11_BLEND_ZERO;
			case BlendFunction::BF_ONE:
				return D3D11_BLEND::D3D11_BLEND_ONE;
			case BlendFunction::BF_SRC_COLOR:
				return D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
			case BlendFunction::BF_INV_SRC_COLOR:
				return D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR;
			case BlendFunction::BF_SRC_APLHA:
				return D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			case BlendFunction::BF_INV_SRC_APLHA:
				return D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			case BlendFunction::BF_DEST_APLHA:
				return D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
			case BlendFunction::BF_INV_DEST_APLHA:
				return D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
			case BlendFunction::BF_DEST_COLOR:
				return D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
			case BlendFunction::BF_INV_DEST_COLOR:
				return D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR;
			case BlendFunction::BF_BLEND_FACTOR:
				return D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR;
			case BlendFunction::BF_INV_BLEND_FACTOR:
				return D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR;
			default:
				return D3D11_BLEND::D3D11_BLEND_ONE;
			}
		}

		static D3D11_BLEND_OP getBlendOperation(const BlendOperation& blendOp)
		{
			switch (blendOp)
			{
			case BlendOperation::BOP_ADD:
				return D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			case BlendOperation::BOP_SUBTRACT:
				return D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT;
			case BlendOperation::BOP_REV_SUBTRACT:
				return D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT;
			case BlendOperation::BOP_MIN:
				return D3D11_BLEND_OP::D3D11_BLEND_OP_MIN;
			case BlendOperation::BOP_MAX:
				return D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
			}
		}

		static DXGI_FORMAT getFormat(TextureFormat format)
		{
			switch (format)
			{
			case Hollow::TF_R8:
				return DXGI_FORMAT_R8_UNORM;
			case Hollow::TF_RG8:
				return DXGI_FORMAT_R8G8_UNORM;
			case Hollow::TF_RGB8:
			case Hollow::TF_RGBA8:
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case Hollow::TF_BGR8:
				return DXGI_FORMAT_B8G8R8X8_UNORM;
			case Hollow::TF_BGRA8:
				return DXGI_FORMAT_B8G8R8A8_UNORM;
			case Hollow::TF_R16:
				return DXGI_FORMAT_R16_UNORM;
			case Hollow::TF_RG16:
				return DXGI_FORMAT_R16G16_UNORM;
			case Hollow::TF_RGB16:
			case Hollow::TF_RGBA16:
				return DXGI_FORMAT_R16G16B16A16_UNORM;
			case Hollow::TF_R32:
				return DXGI_FORMAT_R32_FLOAT;
			case Hollow::TF_RG32:
				return DXGI_FORMAT_R32G32_FLOAT;
			case Hollow::TF_RGB32:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case Hollow::TF_RGBA32:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		}

		static int getPicth(TextureFormat format, UINT32 width)
		{
			switch (format)
			{
			case Hollow::TF_R8:
			case Hollow::TF_R16:
			case Hollow::TF_R32:
				return 1 * width;
			case Hollow::TF_RG8:
			case Hollow::TF_RG16:
			case Hollow::TF_RG32:
				return 2 * width;
			case Hollow::TF_RGB8:
			case Hollow::TF_BGR8:
			case Hollow::TF_RGB16:
			case Hollow::TF_RGB32:
				return 3 * width;
			case Hollow::TF_RGBA8:
			case Hollow::TF_BGRA8:
			case Hollow::TF_RGBA16:
			case Hollow::TF_RGBA32:
				return 4 * width;
			}
		}
	};
}
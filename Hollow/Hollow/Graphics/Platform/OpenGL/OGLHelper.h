#pragma once

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/InputLayout.h"
#include "Hollow/Graphics/HardwareBuffer.h"
#include "Hollow/Graphics/CommonTypes.h"

namespace Hollow {
	class OGLHelper
	{
	public:
		static GLuint getInputLayoutFormat(const INPUT_DATA_TYPE type)
		{
			switch (type)
			{
			case Hollow::Float: 
			case Hollow::Float2: 
			case Hollow::Float3: 
			case Hollow::Float4: 
				return GL_FLOAT;
			case Hollow::Int: 
			case Hollow::Int2: 
			case Hollow::Int3: 
			case Hollow::Int4: 
				return GL_INT;
			case Hollow::Bool: 
				return GL_BOOL;
			}
		}

		static std::string getInputLayoutShaderFormat(const INPUT_DATA_TYPE type)
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

		static unsigned int getSize(const INDEX_FORMAT format)
		{
			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT:
			case Hollow::INDEX_FORMAT::INT:
				return 4;
			}
		}

		static GLuint getFormat(const INDEX_FORMAT format)
		{
			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT: 
				return GL_UNSIGNED_INT;
			case Hollow::INDEX_FORMAT::INT: 
				return GL_INT;
			default: 
				return GL_UNSIGNED_INT;
			}
		}

		static GLint getComparisonFunction(const ComparisonFunction comparisonFunc)
		{
			switch (comparisonFunc)
			{
			case ComparisonFunction::CMP_NEVER: 
				return GL_NEVER;
			case ComparisonFunction::CMP_LESS: 
				return GL_LESS;
			case ComparisonFunction::CMP_EQUAL: 
				return GL_EQUAL;
			case ComparisonFunction::CMP_LEQUAL: 
				return GL_LEQUAL;
			case ComparisonFunction::CMP_GREATER: 
				return GL_GREATER;
			case ComparisonFunction::CMP_NOT_EQUAL: 
				return GL_NOTEQUAL;
			case ComparisonFunction::CMP_ALWAYS: 
				return GL_ALWAYS;
			default: 
				return GL_NEVER;
			}
		}

		static GLuint getAddressingMode(const AddressingMode addressingMode)
		{
			switch (addressingMode)
			{
			case AddressingMode::AM_WRAP: 
				return GL_REPEAT;
			case AddressingMode::AM_CLAMP: 
				return GL_CLAMP_TO_EDGE;
			case AddressingMode::AM_MIRROR: 
				return GL_MIRRORED_REPEAT;
			case AddressingMode::AM_BORDER: 
				return GL_CLAMP_TO_BORDER;
			default: 
				return GL_REPEAT;
			}
		}

		static GLuint getFilteringMode(const FilterMode filteringMode)
		{
			switch (filteringMode)
			{
			case FilterMode::FM_POINT: 
				return GL_NEAREST;
			case FilterMode::FM_LINEAR: 
				return GL_LINEAR;
			default: 
				return GL_LINEAR;
			}
		}

		static GLuint getMinMipFilteringMode(const FilterMode _min, const FilterMode _mip)
		{
			switch (_min)
			{
			case FilterMode::FM_NONE:
			case FilterMode::FM_POINT:
				switch (_mip)
				{
				case FilterMode::FM_LINEAR:
				case FilterMode::FM_ANISOTROPIC:
					return GL_NEAREST_MIPMAP_LINEAR;
				case FilterMode::FM_POINT:
					return GL_NEAREST_MIPMAP_NEAREST;
				case FilterMode::FM_NONE:
					return GL_NEAREST;
				}
				break;
			case FilterMode::FM_LINEAR:
			case FilterMode::FM_ANISOTROPIC:
				switch (_mip)
				{
				case FilterMode::FM_LINEAR:
				case FilterMode::FM_ANISOTROPIC:
					return GL_LINEAR_MIPMAP_LINEAR;
				case FilterMode::FM_POINT:
					return GL_LINEAR_MIPMAP_NEAREST;
				case FilterMode::FM_NONE:
					return GL_LINEAR;
				}
				break;
			}
		}

		static GLuint getDepthStencilOperation(const StencilOperation stecnilOp)
		{
			switch (stecnilOp)
			{
			case StencilOperation::SOP_KEEP: 
				return GL_KEEP;
			case StencilOperation::SOP_ZERO: 
				return GL_ZERO;
			case StencilOperation::SOP_INCR: 
				return GL_INCR;
			case StencilOperation::SOP_INCR_WRAP: 
				return GL_INCR_WRAP;
			case StencilOperation::SOP_DECR: 
				return GL_DECR;
			case StencilOperation::SOP_DECR_WRAP: 
				return GL_DECR_WRAP;
			case StencilOperation::SOP_REPLACE: 
				return GL_REPLACE;
			case StencilOperation::SOP_INVERT: 
				return GL_INVERT;
			}
		}

		static GLuint getBlend(const BlendFunction& blendFunc)
		{
			switch (blendFunc)
			{
			case BlendFunction::BF_ZERO: 
				return GL_ZERO;
			case BlendFunction::BF_ONE: 
				return GL_ONE;
			case BlendFunction::BF_SRC_COLOR: 
				return GL_SRC_COLOR;
			case BlendFunction::BF_INV_SRC_COLOR: 
				return GL_ONE_MINUS_SRC_COLOR;
			case BlendFunction::BF_SRC_APLHA: 
				return GL_SRC_ALPHA;
			case BlendFunction::BF_INV_SRC_APLHA: 
				return GL_ONE_MINUS_SRC_ALPHA;
			case BlendFunction::BF_DEST_APLHA: 
				return GL_DST_ALPHA;
			case BlendFunction::BF_INV_DEST_APLHA: 
				return GL_ONE_MINUS_DST_ALPHA;
			case BlendFunction::BF_DEST_COLOR: 
				return GL_DST_COLOR;
			case BlendFunction::BF_INV_DEST_COLOR: 
				return GL_ONE_MINUS_DST_COLOR;
			case BlendFunction::BF_BLEND_FACTOR: 
				return GL_CONSTANT_COLOR;
			case BlendFunction::BF_INV_BLEND_FACTOR: 
				return GL_ONE_MINUS_CONSTANT_COLOR;
			}
		}

		static GLuint getBlendOperation(const BlendOperation& blendOp)
		{
			switch (blendOp)
			{
			case BlendOperation::BOP_ADD: 
				return GL_FUNC_ADD;
			case BlendOperation::BOP_SUBTRACT: 
				return GL_FUNC_SUBTRACT;
			case BlendOperation::BOP_REV_SUBTRACT: 
				return  GL_FUNC_REVERSE_SUBTRACT;
			case BlendOperation::BOP_MIN: 
				return GL_MIN;
			case BlendOperation::BOP_MAX: 
				return GL_MAX;
			default: 
				return GL_FUNC_ADD;
			}
		}

		static GLuint getTextureTarget(TextureType type, int numSamples, int numFaces)
		{
			switch (type)
			{
			case TextureType::TT_TEXTURE1D:
				if (numFaces <= 1)
					return GL_TEXTURE_1D;
				else
					return GL_TEXTURE_1D_ARRAY;
			case TextureType::TT_TEXTURE2D:
				if (numSamples > 1)
				{
					if (numFaces <= 1)
						return GL_TEXTURE_2D_MULTISAMPLE;
					else
						return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
				}
				else
				{
					if (numFaces <= 1)
						return GL_TEXTURE_2D;
					else
						return GL_TEXTURE_2D_ARRAY;
				}
			case TextureType::TT_TEXTURE3D:
				return GL_TEXTURE_3D;
			case TextureType::TT_TEXTURE_CUBE:
				if (numFaces <= 6)
					return GL_TEXTURE_CUBE_MAP;
				else
					return GL_TEXTURE_CUBE_MAP_ARRAY;
			default:
				return 0;
			};
		}

		static GLuint getTextureFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::TF_R8: 
				return GL_R8;
			case TextureFormat::TF_RG8: 
				return GL_RG8;
			case TextureFormat::TF_RGB8: 
			case TextureFormat::TF_BGR8: 
				return GL_RGB8;
			case TextureFormat::TF_RGBA8: 
			case TextureFormat::TF_BGRA8: 
				return GL_RGBA8;
			case TextureFormat::TF_R16: 
				return GL_R16;
			case TextureFormat::TF_RG16: 
				return GL_RG16;
			case TextureFormat::TF_RGB16: 
				return GL_RGB16;
			case TextureFormat::TF_RGBA16: 
				return GL_RGBA16;
			case TextureFormat::TF_R32: 
				return GL_R32F;
			case TextureFormat::TF_RG32: 
				return GL_RG32F;
			case TextureFormat::TF_RGB32: 
				return GL_RGB32F;
			case TextureFormat::TF_RGBA32: 
				return GL_RGBA32F;
			}
		}

		static GLuint getInternalTextureFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::TF_R8:
			case TextureFormat::TF_R16:
			case TextureFormat::TF_R32:
				return GL_RED;
			case TextureFormat::TF_RG8:
			case TextureFormat::TF_RG16:
			case TextureFormat::TF_RG32:
				return GL_RG;
			case TextureFormat::TF_RGB8:
			case TextureFormat::TF_RGB16:
			case TextureFormat::TF_RGB32:
				return GL_RGB;
			case TextureFormat::TF_RGBA8:
			case TextureFormat::TF_RGBA16:
			case TextureFormat::TF_RGBA32:
				return GL_RGBA;
			case TextureFormat::TF_BGRA8:
				return GL_BGRA;
			case TextureFormat::TF_BGR8:
				return GL_BGR;
			}
		}

		static GLuint getInternalType(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::TF_R8:
			case TextureFormat::TF_RG8:
			case TextureFormat::TF_BGR8:
			case TextureFormat::TF_RGB8:
				return GL_UNSIGNED_BYTE;
			case TextureFormat::TF_RGBA8:
			case TextureFormat::TF_BGRA8:
				return GL_UNSIGNED_INT_8_8_8_8_REV;
			case TextureFormat::TF_R16:
			case TextureFormat::TF_RG16:
			case TextureFormat::TF_RGB16:
			case TextureFormat::TF_RGBA16:
				return GL_UNSIGNED_SHORT;
			case TextureFormat::TF_R32:
			case TextureFormat::TF_RG32:
			case TextureFormat::TF_RGB32:
			case TextureFormat::TF_RGBA32:
				return GL_FLOAT;
			}
		}

		static GLuint getTopology(const PrimitiveTopology topology)
		{
			switch (topology)
			{
			case PrimitiveTopology::PT_LINELIST:
				return GL_LINES;
			case PrimitiveTopology::PT_LINESTRIP:
				return GL_LINE_STRIP;
			case PrimitiveTopology::PT_TRIANGELIST:
				return GL_TRIANGLES;
			case PrimitiveTopology::PT_TRIANGESTRIP:
				return GL_TRIANGLE_STRIP;
			default:
				return GL_TRIANGLES;
			}
		}
	};
}
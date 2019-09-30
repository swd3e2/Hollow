#pragma once

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/Base/InputLayout.h"
#include "Hollow/Graphics/Base/HardwareBuffer.h"
#include "Hollow/Graphics/Base/CommonTypes.h"

namespace Hollow {
	class OGLHelper
	{
	public:
		static GLuint getInputLayoutFormat(const INPUT_DATA_TYPE type)
		{
			GLuint result = GL_FLOAT;

			switch (type)
			{
			case Hollow::Float:
				result = GL_FLOAT;
				break;
			case Hollow::Float2:
				result = GL_FLOAT;
				break;
			case Hollow::Float3:
				result = GL_FLOAT;
				break;
			case Hollow::Float4:
				result = GL_FLOAT;
				break;
			case Hollow::Int:
				result = GL_INT;
				break;
			case Hollow::Int2:
				result = GL_INT;
				break;
			case Hollow::Int3:
				result = GL_INT;
				break;
			case Hollow::Int4:
				result = GL_INT;
				break;
			case Hollow::Bool:
				result = GL_BOOL;
				break;
			}

			return result;
		}

		static std::string getInputLayoutShaderFormat(const INPUT_DATA_TYPE type)
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

		static unsigned int getSize(const INDEX_FORMAT format)
		{
			unsigned int result = 4;

			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT:
			case Hollow::INDEX_FORMAT::INT:
				result = 4;
				break;
			}

			return result;
		}

		static GLuint getFormat(const INDEX_FORMAT format)
		{
			GLuint result;

			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT:
				result = GL_UNSIGNED_INT;
				break;
			case Hollow::INDEX_FORMAT::INT:
				result = GL_INT;
				break;
			default:
				result = GL_UNSIGNED_INT;
			}

			return result;
		}

		static GLint getComparisonFunction(const ComparisonFunction comparisonFunc)
		{
			GLint result;

			switch (comparisonFunc)
			{
			case ComparisonFunction::CMP_NEVER:
				result = GL_NEVER;
				break;
			case ComparisonFunction::CMP_LESS:
				result = GL_LESS;
				break;
			case ComparisonFunction::CMP_EQUAL:
				result = GL_EQUAL;
				break;
			case ComparisonFunction::CMP_LEQUAL:
				result = GL_LEQUAL;
				break;
			case ComparisonFunction::CMP_GREATER:
				result = GL_GREATER;
				break;
			case ComparisonFunction::CMP_NOT_EQUAL:
				result = GL_NOTEQUAL;
				break;
			case ComparisonFunction::CMP_ALWAYS:
				result = GL_ALWAYS;
				break;
			default:
				result = GL_NEVER;
				break;
			}

			return result;
		}

		static GLuint getAddressingMode(const AddressingMode addressingMode)
		{
			GLuint result;

			switch (addressingMode)
			{
			case AddressingMode::AM_WRAP:
				result = GL_REPEAT;
				break;
			case AddressingMode::AM_CLAMP:
				result = GL_CLAMP;
				break;
			case AddressingMode::AM_MIRROR:
				result = GL_MIRRORED_REPEAT;
				break;
			case AddressingMode::AM_BORDER:
				result = GL_CLAMP_TO_BORDER;
				break;
			default:
				result = GL_REPEAT;
			}

			return result;
		}

		static GLuint getFilteringMode(const FilterMode filteringMode)
		{
			GLuint result;

			switch (filteringMode)
			{
			case FilterMode::FM_POINT:
				result = GL_NEAREST;
				break;
			case FilterMode::FM_LINEAR:
				result = GL_LINEAR;
				break;
			default:
				result = GL_LINEAR;
			}

			return result;
		}

		static GLuint getMinMipFilteringMode(const FilterMode _min, const FilterMode _mip)
		{
			GLuint result = GL_NEAREST_MIPMAP_LINEAR;

			switch (_min)
			{
			case FilterMode::FM_NONE:
			case FilterMode::FM_POINT:
				switch (_mip)
				{
				case FilterMode::FM_LINEAR:
				case FilterMode::FM_ANISOTROPIC:
					result = GL_NEAREST_MIPMAP_LINEAR;
					break;
				case FilterMode::FM_POINT:
					result = GL_NEAREST_MIPMAP_NEAREST;
					break;
				case FilterMode::FM_NONE:
					result = GL_NEAREST;
					break;
				}
				break;
			case FilterMode::FM_LINEAR:
			case FilterMode::FM_ANISOTROPIC:
				switch (_mip)
				{
				case FilterMode::FM_LINEAR:
				case FilterMode::FM_ANISOTROPIC:
					result = GL_LINEAR_MIPMAP_LINEAR;
					break;
				case FilterMode::FM_POINT:
					result = GL_LINEAR_MIPMAP_NEAREST;
					break;
				case FilterMode::FM_NONE:
					result = GL_LINEAR;
					break;
				}
				break;
			}

			return result;
		}

		static GLuint getDepthStencilOperation(const StencilOperation stecnilOp)
		{
			GLuint res = GL_KEEP;

			switch (stecnilOp)
			{
			case StencilOperation::SOP_KEEP:
				res = GL_KEEP;
				break;
			case StencilOperation::SOP_ZERO:
				res = GL_ZERO;
				break;
			case StencilOperation::SOP_INCR:
				res = GL_INCR;
				break;
			case StencilOperation::SOP_INCR_WRAP:
				res = GL_INCR_WRAP;
				break;
			case StencilOperation::SOP_DECR:
				res = GL_DECR;
				break;
			case StencilOperation::SOP_DECR_WRAP:
				res = GL_DECR_WRAP;
				break;
			case StencilOperation::SOP_REPLACE:
				res = GL_REPLACE;
				break;
			case StencilOperation::SOP_INVERT:
				res = GL_INVERT;
				break;
			}

			return res;
		}
	};
}
#pragma once

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/Base/InputLayout.h"
#include "Hollow/Graphics/Base/HardwareBuffer.h"

namespace Hollow {
	class OGLHelper
	{
	public:
		static GLuint getInputLayoutFormat(INPUT_DATA_TYPE type)
		{
			GLuint result;

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
			unsigned int result;

			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT:
			case Hollow::INDEX_FORMAT::INT:
				result = 4;
				break;
			}

			return result;
		}

		static GLuint getFormat(INDEX_FORMAT format)
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
			}

			return result;
		}

		static GLint getComparisonFunction(const ComparisonFunction& func)
		{
			GLint result;

			switch (func)
			{
			case NEVER:
				result = GL_NEVER;
				break;
			case LESS:
				result = GL_LESS;
				break;
			case EQUAL:
				result = GL_EQUAL;
				break;
			case LEQUAL:
				result = GL_LEQUAL;
				break;
			case GREATER:
				result = GL_GREATER;
				break;
			case NOT_EQUAL:
				result = GL_NOTEQUAL;
				break;
			case ALWAYS:
				result = GL_ALWAYS;
				break;
			default:
				result = GL_NEVER;
			}

			return result;
		}

		static GLuint getAddressingMode(const AddressingMode& mode)
		{
			GLuint result;

			switch (mode)
			{
			case WRAP:
				result = GL_REPEAT;
				break;
			case CLAMP:
				result = GL_CLAMP;
				break;
			case MIRROR:
				result = GL_MIRRORED_REPEAT;
				break;
			case BORDER:
				result = GL_CLAMP_TO_BORDER;
				break;
			default:
				result = GL_REPEAT;
			}

			return result;
		}

		static GLuint getFilteringMode(const FilterMode& mode)
		{
			GLuint result;

			switch (mode)
			{
			case NONE:
				result = GL_REPEAT;
				break;
			case POINT:
				result = GL_CLAMP;
				break;
			case LINEAR:
				result = GL_MIRRORED_REPEAT;
				break;
			case ANISOTROPIC:
				result = GL_CLAMP_TO_BORDER;
				break;
			default:
				result = GL_REPEAT;
			}

			return result;
		}

		static GLuint getMinMipFilteringMode(const FilterMode& min, const FilterMode& mip)
		{
			GLuint result;

			switch (min)
			{
			case NONE:
			case POINT:
				switch (mip)
				{
				case LINEAR:
				case ANISOTROPIC:
					result = GL_NEAREST_MIPMAP_LINEAR;
				case POINT:
					result = GL_NEAREST_MIPMAP_NEAREST;
				case NONE:
					result = GL_NEAREST;
				default:
					break;
				}
				break;
			case LINEAR:
			case ANISOTROPIC:
				switch (mip)
				{
				case LINEAR:
				case ANISOTROPIC:
					result = GL_LINEAR_MIPMAP_LINEAR;
				case POINT:
					result = GL_LINEAR_MIPMAP_NEAREST;
				case NONE:
					result = GL_LINEAR;
				default:
					break;
				}
				break;
			}

			return result;
		}
	};
}
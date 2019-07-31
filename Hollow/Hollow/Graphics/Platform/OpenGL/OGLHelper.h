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
			switch (type)
			{
				case Hollow::Float:		return GL_FLOAT;
				case Hollow::Float2:	return GL_FLOAT;
				case Hollow::Float3:	return GL_FLOAT;
				case Hollow::Float4:	return GL_FLOAT;
				case Hollow::Int:		return GL_INT;
				case Hollow::Int2:		return GL_INT;
				case Hollow::Int3:		return GL_INT;
				case Hollow::Int4:		return GL_INT;
				case Hollow::Bool:		return GL_BOOL;
			}
		}

		static std::string getInputLayoutShaderFormat(INPUT_DATA_TYPE type)
		{
			switch (type)
			{
				case Hollow::Float:		return "float";
				case Hollow::Float2:	return "float2";
				case Hollow::Float3:	return "float3";
				case Hollow::Float4:	return "float4";
				case Hollow::Int:		return "int";
				case Hollow::Int2:		return "int2";
				case Hollow::Int3:		return "int3";
				case Hollow::Int4:		return "int4";
				case Hollow::Bool:		return "bool";
			}
		}

		static unsigned int getSize(INDEX_FORMAT format)
		{
			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT:	return 4;
			case Hollow::INDEX_FORMAT::INT:		return 4;
			}

			return 0;
		}

		static unsigned int getFormat(INDEX_FORMAT format)
		{
			switch (format)
			{
			case Hollow::INDEX_FORMAT::UINT:	return GL_UNSIGNED_INT;
			case Hollow::INDEX_FORMAT::INT:		return GL_INT;
			}

			return 0;
		}
	};
}
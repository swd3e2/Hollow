#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/InputLayout.h"
#include <string>

namespace Hollow {
	class D3D11Helper 
	{
	public:
		static DXGI_FORMAT getInputLayoutFormat(INPUT_DATA_TYPE type)
		{
			switch (type)
			{
				case Hollow::Float:		return DXGI_FORMAT_R32_FLOAT;
				case Hollow::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
				case Hollow::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
				case Hollow::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
				case Hollow::Int:		return DXGI_FORMAT_R32_SINT;
				case Hollow::Int2:		return DXGI_FORMAT_R32G32_SINT;
				case Hollow::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
				case Hollow::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
				case Hollow::Bool:		return DXGI_FORMAT_R32_UINT;
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
	};
}
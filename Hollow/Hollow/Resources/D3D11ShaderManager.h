#pragma once

#include "ShaderManager.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11VertexShader.h"

namespace Hollow {
	class D3D11ShaderManager : public ShaderManager
	{
	public:
		virtual Shader* create(SHADER_DESC& desc) override
		{
			switch (desc.type)
			{
			case ShaderType::VERTEX: {
				D3D11_INPUT_ELEMENT_DESC bxlayout[] =
				{
					{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,	   0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,	   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,	   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "BITANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "BONEID",     0, DXGI_FORMAT_R32G32B32A32_SINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "WEIGHT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
			} break;
			default:
				break;
			}
		}

		virtual ShaderProgram* createShaderProgram(SHADER_PROGRAM_DESC& desc) override
		{

		}
	};
}
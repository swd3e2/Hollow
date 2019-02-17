#pragma once
#include <d3d11.h>

namespace Hollow {

	class Texture
	{
	public:
		Texture(ID3D11ShaderResourceView* textureShaderResource, std::string name) :
			m_TextureShaderResource(textureShaderResource), name(name)
		{}

		std::string name;
		ID3D11ShaderResourceView* m_TextureShaderResource;
	};

}
#pragma once
#include <d3d11.h>

namespace Hollow {

	class Texture
	{
	public:
		Texture(ID3D11ShaderResourceView* textureShaderResource) :
			m_TextureShaderResource(textureShaderResource)
		{}

		ID3D11ShaderResourceView* m_TextureShaderResource;
	};

}
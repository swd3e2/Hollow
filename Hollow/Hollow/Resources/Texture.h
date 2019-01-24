#pragma once
#include <d3d11.h>

namespace Hollow {

	class Texture
	{
	public:
		Texture(std::string filename)
		{
			
		}
		ID3D11ShaderResourceView * m_TextureShaderResource;
	};

}
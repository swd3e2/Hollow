#pragma once
#include <d3d11.h>

namespace Hollow {

	class Texture
	{
	public:
		ID3D11ShaderResourceView * m_TextureShaderResource;
	};

}
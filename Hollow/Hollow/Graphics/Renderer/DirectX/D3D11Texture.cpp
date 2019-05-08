#include "D3D11Texture.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

D3D11Texture::~D3D11Texture()
{
	SAFE_RELEASE(m_TextureShaderResource);
}

void D3D11Texture::SetShaderResource(ID3D11ShaderResourceView* textureShaderResource)
{
	SAFE_RELEASE(m_TextureShaderResource);
	m_TextureShaderResource = textureShaderResource;
}

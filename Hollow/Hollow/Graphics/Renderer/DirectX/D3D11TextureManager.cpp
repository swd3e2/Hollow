#include "D3D11TextureManager.h"
#include "D3D11RenderApi.h"

Texture* D3D11TextureManager::Create2dTexture(TEXTURE_DESC* desc)
{
	D3D11Texture* texture = new D3D11Texture();

	texture->active = true;

	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	ID3D11Device* device = r->getContext().getDevice();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Height = desc->height;
	textureDesc.Width = desc->width;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = desc->mInitialData;
	initData.SysMemPitch = desc->pitch;
	initData.SysMemSlicePitch = 0;

	ID3D11Texture2D* m_texture;
	// Create the empty texture.
	if (FAILED(device->CreateTexture2D(&textureDesc, &initData, &m_texture))) {
		//HW_ERROR("D3DTexture: Can't create 2D texture");
	}

	// Setup the shader resource view description.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view for the texture.
	if (device->CreateShaderResourceView(m_texture, &srvDesc, &texture->m_TextureShaderResource) != S_OK) {
		//HW_ERROR("D3DTexture: Can't create shader resource view for 2d texture");
	}

	textureList["0"] = texture;

	return texture;
}

Texture* D3D11TextureManager::Create3dTexture(TEXTURE_DESC** desc)
{
	D3D11Texture* texture = new D3D11Texture();

	texture->active = true;

	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	ID3D11Device* device = r->getContext().getDevice();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Height = desc[0]->height;
	textureDesc.Width = desc[0]->width;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA pData[6];

	for (int i = 0; i < 6; i++)
	{
		pData[i].pSysMem = desc[i]->mInitialData;
		pData[i].SysMemPitch = desc[i]->pitch;
		pData[i].SysMemSlicePitch = 0;
	}

	ID3D11Texture2D* m_texture;
	if (device->CreateTexture2D(&textureDesc, &pData[0], &m_texture) != S_OK) {
		//HW_ERROR("D3DTexture: Can't create 2D texture");
	}

	// Setup the shader resource view description.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view for the texture.
	if (device->CreateShaderResourceView(m_texture, &srvDesc, &texture->m_TextureShaderResource) != S_OK) {
		//HW_ERROR("D3DTexture: Can't create shader resource view for 2d texture");
	}

	textureList["0"] = texture;

	return texture;
}

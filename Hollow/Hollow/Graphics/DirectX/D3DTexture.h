#pragma once
#include <d3d11.h>
#include <string>
#include "Hollow/Platform.h"
#include "Hollow/Resources/TextureLoader.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Graphics/ITexture.h"

class D3DTexture : public ITexture
{
public:
	~D3DTexture() 
	{
		SAFE_RELEASE(m_TextureShaderResource);
	}

	void CreateTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename)
	{
		std::string filepath = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/";
		filepath += filename;
		TextureData* data = TextureLoader::loadFromFile(filepath.c_str());
		
		if (data == nullptr) {
			delete data;
			return;
		}

		active = true;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Height = data->height;
		textureDesc.Width = data->width;
		textureDesc.MiscFlags = 0;
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
		initData.pSysMem = data->data;
		initData.SysMemPitch = static_cast<UINT>(data->pitch);
		initData.SysMemSlicePitch = 0;

		ID3D11Texture2D * m_texture;
		// Create the empty texture.
		if (FAILED(device->CreateTexture2D(&textureDesc, &initData, &m_texture))) {
			HW_ERROR("D3DTexture: Can't create 2D texture");
			delete data;
			return;
		}
		delete data;
		// Setup the shader resource view description.
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view for the texture.
		if (FAILED(device->CreateShaderResourceView(m_texture, &srvDesc, &m_TextureShaderResource))) {
			HW_ERROR("D3DTexture: Can't create shader resource view for 2d texture");
			delete data;
			return;
		}

		return;
	}

	void SetShaderResource(ID3D11ShaderResourceView* textureShaderResource)
	{
		SAFE_RELEASE(m_TextureShaderResource);
		m_TextureShaderResource = textureShaderResource;
	}

	ID3D11ShaderResourceView* m_TextureShaderResource;
	bool active = false;
};
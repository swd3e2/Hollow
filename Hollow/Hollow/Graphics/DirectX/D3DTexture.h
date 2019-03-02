#pragma once
#include <d3d11.h>
#include <string>
#include "Hollow/Platform.h"
#include "Hollow/Resources/TextureLoader.h"

class D3DTexture
{
public:
	~D3DTexture() 
	{
		SAFE_RELEASE(m_TextureShaderResource);
	}

	void CreateTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename)
	{
		ID3D11Texture2D * m_texture;
		std::string filepath = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/";
		filepath += filename;
		TextureData* data = TextureLoader::loadFromFile(filepath.c_str());

		if (data == nullptr) {
			return;
		}
		
		active = true;

		HRESULT hResult;
		unsigned int rowPitch;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Height = data->height;	
		textureDesc.Width = data->width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create the empty texture.
		hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
		if (FAILED(hResult))
		{
			return;
		}

		// Copy the targa image data into the texture.
		deviceContext->UpdateSubresource(m_texture, 0, NULL, data->data, data->pitch, 0);

		// Setup the shader resource view description.
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		// Create the shader resource view for the texture.
		hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_TextureShaderResource);
		if (FAILED(hResult))
		{
			return;
		}

		// Generate mipmaps for this texture.
		deviceContext->GenerateMips(m_TextureShaderResource);

		// Release the targa image data now that the image data has been loaded into the texture.
		delete data;

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
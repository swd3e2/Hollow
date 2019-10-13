#include "D3D11TextureManager.h"
#include "D3D11RenderApi.h"

namespace Hollow {
	s_ptr<Texture> D3D11TextureManager::create(const TEXTURE_DESC& desc)
	{
		D3D11Texture* texture = new D3D11Texture(desc);
		
		UINT32 numFaces = desc.type == TextureType::TT_TEXTURE_CUBE ? 6 : 1 * (desc.arraySlices == 0 ? 1 : desc.arraySlices);
		UINT32 numMips = desc.numMips + 1;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();
		ID3D11DeviceContext* deviceContext = r->getContext().getDeviceContext();

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Height = desc.height;
		textureDesc.Width = desc.width;
		textureDesc.MipLevels = numMips;
		textureDesc.ArraySize = numFaces == 0 ? 1 : numFaces;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Format = D3D11Helper::getFormat(desc.format);
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;

		if (desc.type == TextureType::TT_TEXTURE_CUBE) 
		{
			textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		}

		if (desc.numMips >= 1)
		{
			textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		if (device->CreateTexture2D(&textureDesc, nullptr, &texture->m_Texture) != S_OK) {
			return nullptr;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format	;

		switch (desc.type)
		{
		case Hollow::TT_TEXTURE1D:
			if (numFaces > 1) 
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
				srvDesc.Texture1DArray.ArraySize;
				srvDesc.Texture1DArray.FirstArraySlice;
				srvDesc.Texture1DArray.MostDetailedMip = 0;
				srvDesc.Texture1DArray.MipLevels = numMips;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				srvDesc.Texture1D.MostDetailedMip = 0;
				srvDesc.Texture1D.MipLevels = numMips;

			}
		case Hollow::TT_TEXTURE2D:
			if (desc.samples > 1)
			{
				if (numFaces > 1)
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					srvDesc.Texture2DMSArray.ArraySize;
					srvDesc.Texture2DMSArray.FirstArraySlice;
				}
				else
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
				}
			}
			else
			{
				if (numFaces > 1)
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					srvDesc.Texture2DArray.ArraySize;
					srvDesc.Texture2DArray.FirstArraySlice;
					srvDesc.Texture2DArray.MostDetailedMip = 0;
					srvDesc.Texture2DArray.MipLevels = numMips;
				}
				else
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MostDetailedMip = 0;
					srvDesc.Texture2D.MipLevels = numMips;
				}
			}
		case Hollow::TT_TEXTURE3D:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			srvDesc.Texture3D.MostDetailedMip = 0;
			srvDesc.Texture3D.MipLevels = numMips;
		case Hollow::TT_TEXTURE_CUBE:
			if (numFaces > 6) 
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
				srvDesc.TextureCubeArray.First2DArrayFace;
				srvDesc.TextureCubeArray.NumCubes;
				srvDesc.TextureCubeArray.MostDetailedMip = 0;
				srvDesc.TextureCubeArray.MipLevels = numMips;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MostDetailedMip = 0;
				srvDesc.TextureCube.MipLevels = numMips;
			}
		}
		HW_INFO("Cubemap texture not found, filename {}", 123);

		// Create the shader resource view for the texture.
		if (device->CreateShaderResourceView(texture->m_Texture, &srvDesc, &texture->m_TextureShaderResource) != S_OK) {
			return nullptr;
		}

		return s_ptr<Texture>(texture);
	}

	s_ptr<Texture> D3D11TextureManager::create(const TEXTURE_DESC& desc, const s_ptr<Import::Texture>& texture)
	{
		return s_ptr<Texture>();
	}
}
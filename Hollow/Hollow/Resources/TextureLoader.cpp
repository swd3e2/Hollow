#include "TextureLoader.h"

namespace Hollow {

	bool TextureLoader::LoadFromWICFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wchar_t* filename, ID3D11ShaderResourceView ** shaderResourceVie)
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, filename, nullptr, shaderResourceVie);
		// if failed - false
		return !FAILED(hr);
	}

	bool TextureLoader::LoadFromTGAFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wchar_t* filename, ID3D11ShaderResourceView ** shaderResourceView)
	{
		_bstr_t convert(filename);
		char * charFileName = convert;
		return LoadFromTGAFile(device, deviceContext, charFileName, shaderResourceView);
	}

	bool TextureLoader::LoadFromTGAFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, ID3D11ShaderResourceView ** shaderResourceView)
	{
		ID3D11Texture2D * m_texture;
		unsigned char * m_targaData = 0;

		bool result;
		int height, width;
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT hResult;
		unsigned int rowPitch;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		// Load the targa image data into memory.
		result = LoadTarga(filename, height, width, &m_targaData);
		if (!result)
		{
			return false;
		}
		// Setup the description of the texture.
		textureDesc.Height = height;
		textureDesc.Width = width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create the empty texture.
		hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
		if (FAILED(hResult))
		{
			return false;
		}

		// Set the row pitch of the targa image data.
		rowPitch = (width * 4) * sizeof(unsigned char);

		// Copy the targa image data into the texture.
		deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);
		// Setup the shader resource view description.
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		// Create the shader resource view for the texture.
		hResult = device->CreateShaderResourceView(m_texture, &srvDesc, shaderResourceView);
		if (FAILED(hResult))
		{
			return false;
		}

		// Generate mipmaps for this texture.
		deviceContext->GenerateMips(*shaderResourceView);

		// Release the targa image data now that the image data has been loaded into the texture.
		delete[] m_targaData;
		m_targaData = 0;

		return true;
	}

	bool TextureLoader::LoadTarga(char* filename, int& height, int& width, unsigned char ** m_targaData)
	{
		int error, bpp, imageSize, index, i, j, k;
		FILE* filePtr;
		unsigned int count;
		TargaHeader targaFileHeader;
		unsigned char* targaImage;


		// Open the targa file for reading in binary.
		error = fopen_s(&filePtr, filename, "rb");
		if (error != 0)
		{
			return false;
		}

		// Read in the file header.
		count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
		if (count != 1)
		{
			return false;
		}

		// Get the important information from the header.
		height = (int)targaFileHeader.height;
		width = (int)targaFileHeader.width;
		bpp = (int)targaFileHeader.bpp;

		// Check that it is 32 bit and not 24 bit.
		if (bpp != 32)
		{
			return false;
		}

		// Calculate the size of the 32 bit image data.
		imageSize = width * height * 4;

		// Allocate memory for the targa image data.
		targaImage = new unsigned char[imageSize];
		if (!targaImage)
		{
			return false;
		}

		// Read in the targa image data.
		count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
		if (count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if (error != 0)
		{
			return false;
		}

		// Allocate memory for the targa destination data.
		*m_targaData = new unsigned char[imageSize];
		std::string message = std::to_string(imageSize);


		if (!*m_targaData)
		{
			return false;
		}

		// Initialize the index into the targa destination data array.
		index = 0;

		// Initialize the index into the targa image data.
		k = (width * height * 4) - (width * 4);

		// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
		for (j = 0; j < height; j++)
		{
			for (i = 0; i < width; i++)
			{
				(*m_targaData)[index + 0] = targaImage[k + 2];  // Red.
				(*m_targaData)[index + 1] = targaImage[k + 1];  // Green.
				(*m_targaData)[index + 2] = targaImage[k + 0];  // Blue
				(*m_targaData)[index + 3] = targaImage[k + 3];  // Alpha

				// Increment the indexes into the targa data.
				k += 4;
				index += 4;
			}

			// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
			k -= (width * 8);
		}

		// Release the targa image data now that it was copied into the destination array.
		delete[] targaImage;
		targaImage = 0;

		return true;
	}

	ID3D11ShaderResourceView* TextureLoader::LoadTexture(ID3D11Device * m_Device, ID3D11DeviceContext * m_pImmediateContext, char* textureFileName)
	{
		const size_t cSize = strlen(textureFileName) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, textureFileName, cSize);

		std::wstring filename = wc;
		filename.insert(0, L"Data/Textures/");
		return LoadTexture(m_Device, m_pImmediateContext, (wchar_t*)filename.c_str());
	}

	ID3D11ShaderResourceView* TextureLoader::LoadTexture(ID3D11Device * m_Device, ID3D11DeviceContext * m_pImmediateContext, wchar_t* textureFileName)
	{
		ID3D11ShaderResourceView* textureShaderResource = nullptr;
		bool res = false;
		// @todo: better filetype filter
		if (wcslen(textureFileName) > 0) {
			res = LoadFromWICFile(m_Device, m_pImmediateContext, (wchar_t*)textureFileName, &textureShaderResource);
			if (!res) {
				LoadFromTGAFile(m_Device, m_pImmediateContext, (wchar_t*)textureFileName, &textureShaderResource);
			}
		}
		return textureShaderResource;
	}
}

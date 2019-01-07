#pragma once
#include <d3d11.h>
#include <fstream>
#include <string>
#include <comdef.h>
#include "WICTextureLoader.h"
#include "Hollow/Platform.h"

namespace Hollow {

	class HOLLOW_API TextureLoader
	{
	private:
		struct TargaHeader
		{
			unsigned char data1[12];
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char data2;
		};
	public:
		inline bool LoadFromWICFile(ID3D11Device*, ID3D11DeviceContext*, wchar_t*, ID3D11ShaderResourceView **);
		bool LoadFromTGAFile(ID3D11Device*, ID3D11DeviceContext*, wchar_t*, ID3D11ShaderResourceView **);
		bool LoadFromTGAFile(ID3D11Device*, ID3D11DeviceContext*, char*, ID3D11ShaderResourceView **);
		bool LoadTarga(char* filename, int& height, int& width, unsigned char ** m_targaData);
	};

}
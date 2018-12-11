
#ifndef __I_SHADER__
#define __I_SHADER__
#pragma once
#include "windows.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

namespace Hollow {

	class IShader
	{
	protected:
		HRESULT CompileShader(std::wstring filename, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
		{
			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

			ID3DBlob* shaderBlob = nullptr;
			ID3DBlob* errorBlob = nullptr;
			HRESULT hr = D3DCompileFromFile(filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				entryPoint, profile,
				flags, 0, &shaderBlob, &errorBlob);

			if (FAILED(hr))
			{
				if (errorBlob)
				{
					OutputDebugStringA((char*)errorBlob->GetBufferPointer());
					errorBlob->Release();
				}
				if (shaderBlob)
					shaderBlob->Release();

				return hr;
			}
			*blob = shaderBlob;
			return hr;
		}
	};
}

#endif
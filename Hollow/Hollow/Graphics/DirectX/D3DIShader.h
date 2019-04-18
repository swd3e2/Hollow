#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include "Hollow/Platform.h"
#include "Hollow/Common/Helper.h"

class D3DIShader
{
protected:
	std::string name;
protected:
	HRESULT CompileShader(std::string filename, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
	{
		name = filename;
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0;

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		wchar_t * wideStringFilename = Hollow::Helper::converToWideChar(filename.c_str());

		HRESULT hr = D3DCompileFromFile(wideStringFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
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
public:
	inline std::string getName() { return name; }
};

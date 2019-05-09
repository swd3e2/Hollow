#include "D3D11ShaderManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

Shader* D3D11ShaderManager::compileShader(ShaderType type, const std::string& path)
{
	Shader* shader = nullptr;
	switch (type)
	{
	case VERTEX:
	{
		D3D11VertexShader* vShader = new D3D11VertexShader();
		HRESULT hr = CompileShaderInternal(path, "main", "vs_5_0", &vShader->m_ShaderBlob);

		hr = device->CreateVertexShader(vShader->m_ShaderBlob->GetBufferPointer(),
			vShader->m_ShaderBlob->GetBufferSize(), NULL, &vShader->m_Shader);

		D3D11_INPUT_ELEMENT_DESC bxlayout[] =
		{
			{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,	 0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BITANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT numElements = ARRAYSIZE(bxlayout);

		// Create input layout to vertex shader
		hr = device->CreateInputLayout(bxlayout, numElements,
			vShader->m_ShaderBlob->GetBufferPointer(), vShader->m_ShaderBlob->GetBufferSize(),
			&vShader->m_InputLayout);
		shader = vShader;
	} break;
	case PIXEL:
	{
		D3D11PixelShader* pShader = new D3D11PixelShader();

		HRESULT hr = CompileShaderInternal(path, "main", "ps_5_0", &pShader->m_ShaderBlob);
		hr = device->CreatePixelShader(pShader->m_ShaderBlob->GetBufferPointer(),
			pShader->m_ShaderBlob->GetBufferSize(), NULL, &pShader->m_Shader);
		shader = pShader;
	} break;
	case GEOMERTY:
	{
		D3D11GeometryShader* gShader = new D3D11GeometryShader();

		HRESULT hr = CompileShaderInternal(path, "main", "ps_5_0", &gShader->m_ShaderBlob);
		hr = device->CreateGeometryShader(gShader->m_ShaderBlob->GetBufferPointer(),
			gShader->m_ShaderBlob->GetBufferSize(), NULL, &gShader->m_Shader);
		shader = gShader;
	} break;
	case COMPUTE:
	{
		D3D11ComputeShader* cShader = new D3D11ComputeShader();

		HRESULT hr = CompileShaderInternal(path, "main", "cs_5_0", &cShader->m_ShaderBlob);
		hr = device->CreateComputeShader(cShader->m_ShaderBlob->GetBufferPointer(),
			cShader->m_ShaderBlob->GetBufferSize(), NULL, &cShader->m_Shader);
		shader = cShader;
	} break;
	case HULL:
	{
		D3D11HullShader* cShader = new D3D11HullShader();

		HRESULT hr = CompileShaderInternal(path, "main", "hs_5_0", &cShader->m_ShaderBlob);
		hr = device->CreateHullShader(cShader->m_ShaderBlob->GetBufferPointer(),
			cShader->m_ShaderBlob->GetBufferSize(), NULL, &cShader->m_Shader);
		shader = cShader;
	} break;
	case DOMAINS:
	{
		D3D11DomainShader* cShader = new D3D11DomainShader();

		HRESULT hr = CompileShaderInternal(path, "main", "ds_5_0", &cShader->m_ShaderBlob);
		hr = device->CreateDomainShader(cShader->m_ShaderBlob->GetBufferPointer(),
			cShader->m_ShaderBlob->GetBufferSize(), NULL, &cShader->m_Shader);
		shader = cShader;
	} break;
	default:
		break;
	}
	return shader;
}

HRESULT D3D11ShaderManager::CompileShaderInternal(const std::string& path, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0;

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	wchar_t* wideStringFilename = Hollow::Helper::converToWideChar(path.c_str());

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

D3D11ShaderManager::D3D11ShaderManager()
{
	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	device = r->getContext().getDevice();

	std::vector<std::string>* shaders;

	shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/");

	for (auto& it : *shaders)
	{
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
		{
			ShaderProgram* shader = new ShaderProgram();
			shader->setPixelShader(compileShader(ShaderType::PIXEL, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/" + it));
			shader->setVertexShader(compileShader(ShaderType::VERTEX, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/" + it));
			this->shaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = shader;
		}
	}
}

D3D11ShaderManager::~D3D11ShaderManager()
{

}

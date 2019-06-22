#include "D3D11ShaderManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	Shader* D3D11ShaderManager::compileShader(ShaderType type, const std::string& path, Shader* prevShader)
	{
		Shader* shader = nullptr;

		switch (type)
		{
		case VERTEX:
		{
			ID3DBlob* shaderBlob;
			D3D11VertexShader* vShader = nullptr;

			if (prevShader != nullptr) {
				vShader = static_cast<D3D11VertexShader*>(prevShader);
			}
			else {
				vShader = new D3D11VertexShader();
			}

			if (CompileShaderInternal(path, "main", "vs_5_0", &shaderBlob) == S_OK) {
				vShader->release();

				device->CreateVertexShader(shaderBlob->GetBufferPointer(),
					shaderBlob->GetBufferSize(), NULL, &vShader->m_Shader);

				D3D11_INPUT_ELEMENT_DESC bxlayout[] =
				{
					{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,	   0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,	   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,	   0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "BITANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "BONEID",     0, DXGI_FORMAT_R32G32B32A32_SINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "WEIGHT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				UINT numElements = ARRAYSIZE(bxlayout);

				// Create input layout to vertex shader
				device->CreateInputLayout(bxlayout, numElements,
					shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
					&vShader->m_InputLayout);
				shader = vShader;
			}
		} break;
		case PIXEL:
		{
			ID3DBlob* shaderBlob;
			D3D11PixelShader* pShader = nullptr;

			if (prevShader != nullptr) {
				pShader = static_cast<D3D11PixelShader*>(prevShader);
			}
			else {
				pShader = new D3D11PixelShader();
			}

			if (CompileShaderInternal(path, "main", "ps_5_0", &shaderBlob) == S_OK) {
				pShader->release();

				device->CreatePixelShader(shaderBlob->GetBufferPointer(),
					shaderBlob->GetBufferSize(), NULL, &pShader->m_Shader);
				shader = pShader;
			}
		} break;
		case GEOMERTY:
		{
			ID3DBlob* shaderBlob;
			D3D11GeometryShader* gShader = nullptr;

			if (prevShader != nullptr) {
				gShader = static_cast<D3D11GeometryShader*>(prevShader);
			}
			else {
				gShader = new D3D11GeometryShader();
			}

			if (CompileShaderInternal(path, "main", "ps_5_0", &shaderBlob) == S_OK) {
				gShader->release();

				device->CreateGeometryShader(shaderBlob->GetBufferPointer(),
					shaderBlob->GetBufferSize(), NULL, &gShader->m_Shader);
				shader = gShader;
			}
		} break;
		case COMPUTE:
		{
			ID3DBlob* shaderBlob;
			D3D11ComputeShader* cShader = nullptr;

			if (prevShader != nullptr) {
				cShader = static_cast<D3D11ComputeShader*>(prevShader);
			}
			else {
				cShader = new D3D11ComputeShader();
			}

			if (CompileShaderInternal(path, "main", "cs_5_0", &shaderBlob) == S_OK) {
				cShader->release();

				device->CreateComputeShader(shaderBlob->GetBufferPointer(),
					shaderBlob->GetBufferSize(), NULL, &cShader->m_Shader);
				shader = cShader;
			}
		} break;
		case HULL:
		{
			ID3DBlob* shaderBlob;
			D3D11HullShader* cShader = nullptr;

			if (prevShader != nullptr) {
				cShader = static_cast<D3D11HullShader*>(prevShader);
			}
			else {
				cShader = new D3D11HullShader();
			}

			if (CompileShaderInternal(path, "main", "hs_5_0", &shaderBlob) == S_OK) {
				cShader->release();

				device->CreateHullShader(shaderBlob->GetBufferPointer(),
					shaderBlob->GetBufferSize(), NULL, &cShader->m_Shader);
				shader = cShader;
			}
		} break;
		case DOMAINS:
		{
			ID3DBlob* shaderBlob;
			D3D11DomainShader* cShader = nullptr;

			if (prevShader != nullptr) {
				cShader = static_cast<D3D11DomainShader*>(prevShader);
			}
			else {
				cShader = new D3D11DomainShader();
			}

			if (CompileShaderInternal(path, "main", "ds_5_0", &shaderBlob) == S_OK) {
				cShader->release();

				device->CreateDomainShader(shaderBlob->GetBufferPointer(),
					shaderBlob->GetBufferSize(), NULL, &cShader->m_Shader);
				shader = cShader;
			}
		} break;
		default:
			break;
		}
		return shader;
	}

	HRESULT D3D11ShaderManager::CompileShaderInternal(const std::string& path, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

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
				HW_ERROR("{}", (char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (shaderBlob)
				shaderBlob->Release();

			return hr;
		}
		*blob = shaderBlob;
		return hr;
	}

	ShaderProgram* D3D11ShaderManager::createShader(Shader* vertexShader, Shader* pixelShader, ShaderProgram* prevProgram)
	{
		return new D3D11ShaderProgram(vertexShader, pixelShader);
	}

	void D3D11ShaderManager::reloadShader(ShaderProgram* program)
	{
		std::string shaderName;

		for (auto& it : shaders) {
			if (it.second == program) {
				shaderName = it.first;
			}
		}

		if (!shaderName.size()) {
			return;
		}

		program->setVertexShader(compileShader(ShaderType::VERTEX, shaderFolder + shaderTypeFolder + "/vertex/" + shaderName + ".hlsl", program->getVertexShader()));
		program->setPixelShader(compileShader(ShaderType::PIXEL, shaderFolder + shaderTypeFolder + "/pixel/" + shaderName + ".hlsl", program->getPixelShader()));
	}

	D3D11ShaderManager::D3D11ShaderManager()
	{
	}

	
	D3D11ShaderManager::~D3D11ShaderManager()
	{
		for (auto& it : shaders) {
			delete it.second;
		}
	}

	void D3D11ShaderManager::loadShadersFromFolder()
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		device = r->getContext().getDevice();

		shaderTypeFolder = "D3D11";

		std::vector<std::string> shaders = fs.read_directory(shaderFolder + "/" + shaderTypeFolder + "/vertex/");

		for (auto& it : shaders)
		{
			if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
			{
				ShaderProgram* shader = createShader(
					compileShader(ShaderType::VERTEX, shaderFolder + shaderTypeFolder + "/vertex/" + it),
					compileShader(ShaderType::PIXEL, shaderFolder + shaderTypeFolder + "/pixel/" + it)
				);
				this->shaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = shader;
			}
		}
	}
}
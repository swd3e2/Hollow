#include "D3D11InputLayoutManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"
#include "D3D11Helper.h"
#include <d3dcompiler.h>

namespace Hollow {

	s_ptr<InputLayout> Hollow::D3D11InputLayoutManager::create(const INPUT_LAYOUT_DESC& desc)
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11DeviceContext* deviceContext = r->getContext().getDeviceContext();
		ID3D11Device* device = r->getContext().getDevice();

		D3D11InputLayout* inputLayout = new D3D11InputLayout();
		inputLayout->layout = desc.layout;

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;

		std::string temp = "struct VertexShaderInput {";
		for (int i = 0; i < desc.layout.size(); i++) {
			temp += D3D11Helper::getInputLayoutShaderFormat(desc.layout[i].type) + " attr" + std::to_string(i) + ": " + desc.layout[i].name + ";";
		}

		temp += "}; float4 main(VertexShaderInput input) : SV_POSITION { return float4(1.0f, 1.0f, 1.0f, 1.0f); }";


		const D3D_SHADER_MACRO defines[] =
		{
			{ "HLSL", "1" },
			{ nullptr, nullptr }
		};
		HRESULT hr = D3DCompile(temp.data(), temp.size(), NULL, defines, NULL, "main",
			"vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS,
			0, &shaderBlob, &errorBlob);

		ID3D11VertexShader* vertexTempShader;
		device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexTempShader);

		D3D11_INPUT_ELEMENT_DESC* layout = new D3D11_INPUT_ELEMENT_DESC[desc.layout.size()];

		for (int i = 0; i < desc.layout.size(); i++) {
			if (i == 0) {
				layout[i] = { desc.layout[i].name.c_str(), 0, D3D11Helper::getInputLayoutFormat(desc.layout[i].type), 0,							0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			} else {
				layout[i] = { desc.layout[i].name.c_str(), 0, D3D11Helper::getInputLayoutFormat(desc.layout[i].type), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		}

		device->CreateInputLayout(layout, desc.layout.size(),
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			&inputLayout->m_InputLayout);

		SAFE_RELEASE(errorBlob);
		SAFE_RELEASE(vertexTempShader);
		SAFE_RELEASE(shaderBlob);
		delete[] layout;

		return s_ptr<InputLayout>(inputLayout);
	}

}

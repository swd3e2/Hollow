#include "D3D11RenderStateManager.h"
#include "D3D11Context.h"
#include "D3D11RenderApi.h"

namespace Hollow {
	s_ptr<SamplerState> D3D11RenderStateManager::createSamplerState(const SAMPLER_STATE_DESC& desc)
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		D3D11SamplerState* samplerState = new D3D11SamplerState();

		D3D11_SAMPLER_DESC samplerDesc = {};

		samplerDesc.AddressU = D3D11Helper::getAddressingMode(desc.addressingMode);
		samplerDesc.AddressV = D3D11Helper::getAddressingMode(desc.addressingMode);
		samplerDesc.AddressW = D3D11Helper::getAddressingMode(desc.addressingMode);
		samplerDesc.ComparisonFunc = D3D11Helper::getComparisonFunction(desc.comparisonFunction);
		samplerDesc.MinLOD = desc.minLOD;
		samplerDesc.MaxLOD = desc.maxLOD;
		samplerDesc.MipLODBias = desc.mipLODBias;
		samplerDesc.Filter = D3D11Helper::getFilteringMode(desc.minFilterModel, desc.magFilterMode, desc.mipFilterMode, desc.comparisonFunction != ComparisonFunction::CMP_NEVER);

		if (FAILED(device->CreateSamplerState(&samplerDesc, &samplerState->m_SamplerState))) {
			HW_ERROR("Failed to create sampler state");
		}

		return s_ptr<SamplerState>(samplerState);
	}

	s_ptr<RasterizerState> D3D11RenderStateManager::createRasterizerState(const RASTERIZER_STATE_DESC& desc)
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		D3D11RasterizerState* rasterizerState = new D3D11RasterizerState();
		
		D3D11_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc.CullMode = D3D11Helper::getCullMode(desc.cullMode);
		rasterizerDesc.FillMode = D3D11Helper::getFillMode(desc.fillMode);
		rasterizerDesc.FrontCounterClockwise = desc.frontCounterClockwise;
		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.DepthBias = 0.0001f;
		rasterizerDesc.DepthBiasClamp = 0;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.SlopeScaledDepthBias = 1.0f;

		if (FAILED(device->CreateRasterizerState(&rasterizerDesc, &rasterizerState->m_RasterizerState))) {
			HW_ERROR("Failed to create rasterizer state");
		}

		return s_ptr<RasterizerState>(rasterizerState);
	}

	s_ptr<BlendState> D3D11RenderStateManager::createBlendState(const BLEND_STATE_DESC& desc)
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		D3D11BlendState* blendState = new D3D11BlendState();

		D3D11_BLEND_DESC blendDesc = { 0 };
		blendDesc.AlphaToCoverageEnable = desc.alphaToCoverageEnable;
		blendDesc.IndependentBlendEnable = desc.independentBlendEnable;

		for (int i = 0; i < 8; i++) {
			blendDesc.RenderTarget[i].BlendEnable = desc.blend[i].blendEnabled;
			blendDesc.RenderTarget[i].SrcBlend = D3D11Helper::getBlend(desc.blend[i].srcBlend);
			blendDesc.RenderTarget[i].DestBlend = D3D11Helper::getBlend(desc.blend[i].dstBlend);
			blendDesc.RenderTarget[i].BlendOp = D3D11Helper::getBlendOperation(desc.blend[i].blendOp);
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11Helper::getBlend(desc.blend[i].srcBlendAlpha);
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11Helper::getBlend(desc.blend[i].dstBlendAlpha);
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11Helper::getBlendOperation(desc.blend[i].blendOpAlpha);
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		if (FAILED(device->CreateBlendState(&blendDesc, &blendState->m_BlendState))) {
			HW_ERROR("Failed to create blend state");
		}

		return s_ptr<BlendState>(blendState);
	}

	s_ptr<DepthStencil> D3D11RenderStateManager::createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc)
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		D3D11DepthStencil* depthStencil = new D3D11DepthStencil();

		D3D11_DEPTH_STENCIL_DESC dssDesc = { 0 };
		dssDesc.DepthEnable = desc.depthEnable;
		dssDesc.DepthWriteMask = D3D11Helper::getDepthWriteMask(desc.depthWriteMask);
		dssDesc.DepthFunc = D3D11Helper::getComparisonFunction(desc.depthFunc);

		if (desc.stencilEnable) {
			dssDesc.StencilEnable = desc.stencilEnable;
			dssDesc.StencilReadMask = desc.stencilReadMask;
			dssDesc.StencilWriteMask = desc.stencilWriteMask;

			dssDesc.FrontFace.StencilDepthFailOp = D3D11Helper::getDepthStencilOperation(desc.front.depthFailOp);
			dssDesc.FrontFace.StencilFailOp = D3D11Helper::getDepthStencilOperation(desc.front.failOp);
			dssDesc.FrontFace.StencilPassOp = D3D11Helper::getDepthStencilOperation(desc.front.passOp);
			dssDesc.FrontFace.StencilFunc = D3D11Helper::getComparisonFunction(desc.front.stencilFunc);

			dssDesc.BackFace.StencilDepthFailOp = D3D11Helper::getDepthStencilOperation(desc.back.depthFailOp);
			dssDesc.BackFace.StencilFailOp = D3D11Helper::getDepthStencilOperation(desc.back.failOp);
			dssDesc.BackFace.StencilPassOp = D3D11Helper::getDepthStencilOperation(desc.back.passOp);
			dssDesc.BackFace.StencilFunc = D3D11Helper::getComparisonFunction(desc.back.stencilFunc);
		}

		if (FAILED(device->CreateDepthStencilState(&dssDesc, &depthStencil->depthStencilState))) {
			HW_ERROR("Failed to create depth stencil state");
		}

		return s_ptr<DepthStencil>(depthStencil);
	}
}

